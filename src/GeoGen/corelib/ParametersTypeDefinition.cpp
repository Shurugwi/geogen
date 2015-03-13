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

#include "ParametersTypeDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/ScriptParameter.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/BooleanScriptParameter.hpp"
#include "BooleanTypeDefinition.hpp"
#include "../runtime/NumberScriptParameter.hpp"
#include "NumberTypeDefinition.hpp"
#include "../runtime/EnumScriptParameter.hpp"
#include "EnumTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;
using namespace corelib;

ParametersTypeDefinition::ParametersTypeDefinition(ScriptParameters const& parameters) : TypeDefinition(GG_STR("Parameters"))
{
	for (ScriptParameters::const_iterator it = parameters.Begin(); it != parameters.End(); it++)
	{
		this->GetStaticVariableDefinitions().AddItem(new VariableDefinition(it->first, true));
	}
}

void ParametersTypeDefinition::Initialize(VirtualMachine* vm) const
{
	TypeDefinition::Initialize(vm);

	ManagedObject* staticObject = vm->GetStaticInstance(this->GetName());
	if (staticObject == NULL)
	{
		throw InternalErrorException(GG_STR("Paramters type not initialized properly (static instance missing)."));
	}

	ScriptParameters formalParameters = vm->GetCompiledScript().CreateScriptParameters();

	for (ScriptParameters::iterator it = formalParameters.Begin(); it != formalParameters.End(); it++)
	{
		ScriptParameter const* actualArgument = vm->GetArguments().GetItem(it->first);

		switch (it->second->GetType())
		{	
		case SCRIPT_PARAMETER_TYPE_BOOLEAN:
			{
				BooleanScriptParameter const* typedParameter = dynamic_cast<BooleanScriptParameter const*>(actualArgument == NULL ? it->second : actualArgument);
				ManagedObject* object = vm->GetBooleanTypeDefinition()->CreateInstance(vm, typedParameter->GetValue());
				if (!staticObject->GetMemberVariableTable().DeclareVariable(it->first, object, true))
				{
					throw InternalErrorException(GG_STR("Parameter member name conflict."));
				}
				break;
			}
		case SCRIPT_PARAMETER_TYPE_NUMBER:
			{
				NumberScriptParameter const* typedParameter = dynamic_cast<NumberScriptParameter const*>(actualArgument == NULL ? it->second : actualArgument);
				ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, typedParameter->GetValue());
				if (!staticObject->GetMemberVariableTable().DeclareVariable(it->first, object, true))
				{
					throw InternalErrorException(GG_STR("Parameter member name conflict."));
				}
				break;
			}	
		case SCRIPT_PARAMETER_TYPE_ENUM:
			{
				EnumScriptParameter const* typedParameter = dynamic_cast<EnumScriptParameter const*>(actualArgument == NULL ? it->second : actualArgument);
				ManagedObject* object = typedParameter->GetEnumType()->CreateInstance(vm, typedParameter->GetValue());
				if (!staticObject->GetMemberVariableTable().DeclareVariable(it->first, object, true))
				{
					throw InternalErrorException(GG_STR("Parameter member name conflict."));
				}
				break;
			}	
		default:
			throw InternalErrorException(GG_STR("Invalid script parameter type."));
		}
	}

	if (vm->GetArguments().GetMapWidth() != MAP_SIZE_INFINITE){
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetMapWidth());
		if (!staticObject->GetMemberVariableTable().DeclareVariable(GG_STR("MapWidth"), object, true))
		{
			throw InternalErrorException(GG_STR("MapWidth parameter member name conflict."));
		}
	}

	if (vm->GetArguments().GetMapHeight() != MAP_SIZE_INFINITE){
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetMapHeight());
		if (!staticObject->GetMemberVariableTable().DeclareVariable(GG_STR("MapHeight"), object, true))
		{
			throw InternalErrorException(GG_STR("MapHeight parameter member name conflict."));
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderOriginX());
		if (!staticObject->GetMemberVariableTable().DeclareVariable(GG_STR("RenderOriginX"), object, true))
		{
			throw InternalErrorException(GG_STR("RenderOriginX parameter member name conflict."));
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderOriginY());
		if (!staticObject->GetMemberVariableTable().DeclareVariable(GG_STR("RenderOriginY"), object, true))
		{
			throw InternalErrorException(GG_STR("RenderOriginY parameter member name conflict."));
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderWidth());
		if (!staticObject->GetMemberVariableTable().DeclareVariable(GG_STR("RenderWidth"), object, true))
		{
			throw InternalErrorException(GG_STR("RenderWidth parameter member name conflict."));
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderHeight());
		if (!staticObject->GetMemberVariableTable().DeclareVariable(GG_STR("RenderHeight"), object, true))
		{
			throw InternalErrorException(GG_STR("RenderHeight parameter member name conflict."));
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderScale());
		if (!staticObject->GetMemberVariableTable().DeclareVariable(GG_STR("RenderScale"), object, true))
		{
			throw InternalErrorException(GG_STR("RenderScale parameter member name conflict."));
		}
	}
}

ManagedObject* ParametersTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	return a;
}