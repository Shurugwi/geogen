/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "VirtualMachine.hpp"
#include "TypeDefinition.hpp"
#include "VariableDefinition.hpp"
#include "StaticObject.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"
#include "../ApiUsageException.hpp"
#include "../InternalErrorException.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../renderer/Renderer.hpp"
#include "VirtualMachineStatusGuard.hpp"
#include "CodeBlockStackEntry.hpp"
#include "../CodeLocation.hpp"
#include "UndefinedSymbolAccessException.hpp"
#include "MainMapNotGeneratedException.hpp"
#include "../utils/StringUtils.hpp"
#include "RenderingSequenceTooLongException.hpp"
#include "../renderer/RenderingStep.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;
using namespace utils;
using namespace renderer;

const ScriptParameters VirtualMachine::SCRIPT_PARAMETERS_DEFAULT = ScriptParameters();

VirtualMachine::VirtualMachine(CompiledScript const& compiledScript, ScriptParameters const& arguments)
: 
	compiledScript(compiledScript), 
	status(VIRTUAL_MACHINE_STATUS_READY), 
	globalVariableTable(&memoryManager), 
	arguments(arguments), 
	renderingSequence(arguments.GetRenderScale()), 
	scriptMessageHandler(DefaultScriptMessageHandler), 
	commonRandomSequence(arguments.GetRandomSeed()),
	instructionCounter(0),
	callbackData(NULL)
{
	this->ValidateArguments();
	this->InitializeTypes();
	this->InitializeGlobalVariables();
	this->InitializeMainFunction();
};

void VirtualMachine::InitializeTypes()
{
	for (
		SymbolDefinitionTable<TypeDefinition>::const_iterator it = this->GetCompiledScript().GetTypeDefinitions().Begin(); 
		it != this->GetCompiledScript().GetTypeDefinitions().End(); 
		it++)
	{
		it->second->Initialize(this);
	}
}

void VirtualMachine::DefaultScriptMessageHandler(VirtualMachine* virtualMachine, CodeLocation location, String const& formattedMessage, String const& unformattedMessage, std::vector<String> arguments)
{
	wcout << "Script message: " << StringToWstring(formattedMessage) << endl;
}

void VirtualMachine::EmptyScriptMessageHandler(VirtualMachine* virtualMachine, CodeLocation location, String const& formattedMessage, String const& unformattedMessage, std::vector<String> arguments)
{
}

void VirtualMachine::InitializeGlobalVariables()
{
	for (
		SymbolDefinitionTable<VariableDefinition>::const_iterator it = this->GetCompiledScript().GetGlobalVariableDefinitions().Begin();
		it != this->GetCompiledScript().GetGlobalVariableDefinitions().End();
		it++)
	{
		it->second->Initialize(this);
	}
}

void VirtualMachine::InitializeMainFunction()
{
	FunctionDefinition const* mainFunctionDefinition = this->GetCompiledScript().GetGlobalFunctionDefinitions().GetItem(CompiledScript::MAIN_FUNCTION_NAME);
	if (mainFunctionDefinition == NULL)
	{
		throw InternalErrorException(GG_STR("The script doesn't have a main function."));
	}

	this->CallFunction(CodeLocation(0, 0), mainFunctionDefinition, NULL, 0);
}

void VirtualMachine::ValidateArguments()
{
	ScriptParameters originalParameters = this->GetCompiledScript().CreateScriptParameters();

	if (
		originalParameters.GetDefaultMapWidth() != originalParameters.GetDefaultMapWidth() ||
		originalParameters.GetMinMapWidth() != originalParameters.GetMinMapWidth() ||
		originalParameters.GetMaxMapWidth() != originalParameters.GetMaxMapWidth() ||
		originalParameters.GetDefaultMapHeight() != originalParameters.GetDefaultMapHeight() ||
		originalParameters.GetMinMapHeight() != originalParameters.GetMinMapHeight() ||
		originalParameters.GetMaxMapHeight() != originalParameters.GetMaxMapHeight()
		)
	{
		throw ApiUsageException(GG_STR("Map size defaults/min/max don't match defaults/min/max declared by the script."));
	}

	for (ScriptParameters::const_iterator it = this->GetArguments().Begin(); it != this->GetArguments().End(); it++)
	{
		ScriptParameter* originalParameter = originalParameters.GetItem(it->first);
		if (originalParameter == NULL)
		{
			StringStream ss;
			ss << "Argument \"" << it->first << "\" was not declared by the script.";
			throw ApiUsageException(ss.str());
		}

		if (!it->second->EqualsTo(originalParameter))
		{
			StringStream ss;
			ss << "Argument \"" << it->first << "\" does not match the corresponding formal parameter declared by the script.";
			throw ApiUsageException(ss.str());
		}
	}
}

VirtualMachineStepResult VirtualMachine::Step()
{
	if (this->status != VIRTUAL_MACHINE_STATUS_READY)
	{
		throw ApiUsageException(GG_STR("The VM is in incorrect state."));
	}

	VirtualMachineStatusGuard statusGuard(this->status);

	CallStackEntry& callStackEntry = this->callStack.Top();

	this->instructionCounter++;
	CallStackEntryStepResult stepResult = callStackEntry.Step(this);
	
	if (stepResult == CALL_STACK_ENTRY_STEP_RESULT_FINISHED)
	{
		callStack.Pop();
	}

	VirtualMachineStepResult result;
	if (!callStack.IsEmpty())
	{		
		result = VIRTUAL_MACHINE_STEP_RESULT_RUNNING;
		statusGuard.SetGuardStatus(VIRTUAL_MACHINE_STATUS_READY);
	}
	else
	{		
		result = VIRTUAL_MACHINE_STEP_RESULT_FINISHED;

		this->Finish();

		statusGuard.SetGuardStatus(VIRTUAL_MACHINE_STATUS_FINISHED);
	}

	return result;
}

void VirtualMachine::CallFunction(CodeLocation location, FunctionDefinition const* functionDefinition, ManagedObject* instance, unsigned numberOfArguments)
{
	this->callStack.Push(location, functionDefinition);

	functionDefinition->Call(location, this, instance, numberOfArguments);
}

void VirtualMachine::Run()
{
	while (this->status == VIRTUAL_MACHINE_STATUS_READY)
	{
		this->Step();
	}
}

ManagedObject* VirtualMachine::GetNull()
{
	VariableTableItem* variableTableItem = this->GetGlobalVariableTable().GetVariable(GG_STR("null"));

	if (variableTableItem == NULL)
	{
		throw InternalErrorException(GG_STR("Could not get \"null\" value."));
	}

	return variableTableItem->GetValue();
}

TypeDefinition const* VirtualMachine::GetTypeDefinition(String const& typeName) const
{
	TypeDefinition const* typeDefinition = this->GetCompiledScript().GetTypeDefinitions().GetItem(typeName);
	if (typeDefinition == NULL)
	{
		StringStream ss;
		ss << "Could not get \"" << typeName << "\" type definition.";

		throw InternalErrorException(ss.str());
	}

	return typeDefinition;
}


corelib::BooleanTypeDefinition const* VirtualMachine::GetBooleanTypeDefinition() const
{
	return dynamic_cast<corelib::BooleanTypeDefinition const*>(this->GetTypeDefinition(GG_STR("Boolean")));
}

corelib::NumberTypeDefinition const* VirtualMachine::GetNumberTypeDefinition() const
{
	return dynamic_cast<corelib::NumberTypeDefinition const*>(this->GetTypeDefinition(GG_STR("Number")));
}

VariableTableItem* VirtualMachine::FindVariable(String const& variableName)
{
	if (this->status != VIRTUAL_MACHINE_STATUS_READY)
	{
		throw ApiUsageException(GG_STR("The VM is in incorrect state."));
	}

	CodeBlockStack& codeBlockStack = this->GetCallStack().Top().GetCodeBlockStack();

	VariableTableItem* foundVariable = NULL;
	CodeBlockStack::reverse_iterator it = codeBlockStack.RBegin();
	while (foundVariable == NULL && it != codeBlockStack.REnd())
	{
		if ((*it)->GetLocalVariableTable().IsVariableDeclared(variableName))
		{
			foundVariable = (*it)->GetLocalVariableTable().GetVariable(variableName);
		}

		it++;
	}

	// Not found among local variables, look into global variables.
	if (foundVariable == NULL)
	{
		if (this->GetGlobalVariableTable().IsVariableDeclared(variableName))
		{
			foundVariable = this->GetGlobalVariableTable().GetVariable(variableName);
		}
	}

	return foundVariable;
}

ManagedObject* VirtualMachine::GetStaticInstance(String const& typeName)
{
	VariableTableItem* variableTableItem = this->GetGlobalVariableTable().GetVariable(typeName);

	if (variableTableItem == NULL)
	{
		return NULL;
	}

	else return variableTableItem->GetValue();
}

void VirtualMachine::Finish()
{
	if (this->GetCompiledScript().GetConfiguration().MainMapIsMandatory)
	{
		if (find(this->GetGeneratedMaps().begin(), this->GetGeneratedMaps().end(), renderer::Renderer::MAP_NAME_MAIN) == this->GetGeneratedMaps().end())
		{
			throw MainMapNotGeneratedException(CodeLocation(StringToLines(this->GetCompiledScript().GetCode()).size(), 0));
		}
	}
}

void VirtualMachine::AddRenderingStep(CodeLocation location, RenderingStep* renderingStep)
{
	if (!this->GetRenderingSequence().AddStep(renderingStep))
	{
		throw RenderingSequenceTooLongException(location);
	}
}

void VirtualMachine::Serialize(IOStream& stream) const
{
	stream << "Status: " << status << endl;

	stream << "MemoryManager: " << endl; 
	this->memoryManager.SerializeWithTabs(stream, 1);

	stream << "CallStack: " << endl;
	this->callStack.SerializeWithTabs(stream, 1);

	stream << "CompiledScript: " << endl;
	this->compiledScript.SerializeWithTabs(stream, 1);

	stream << "ObjectStack: " << endl;
	this->objectStack.SerializeWithTabs(stream, 1);

	stream << "GlobalVariableTable: " << endl;
	this->globalVariableTable.SerializeWithTabs(stream, 1);

	stream << "Arguments: " << endl;
	this->arguments.SerializeWithTabs(stream, 1);

	//stream << "RendererObjectSlotTable: " << endl;
	//this->rendererObjectSlotTable.Serialize(stream);
	
	stream << "RenderingSequence: " << endl;
	this->renderingSequence.SerializeWithTabs(stream, 1);
}