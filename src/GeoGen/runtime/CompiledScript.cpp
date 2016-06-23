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

#include "CompiledScript.hpp"
#include "Library.hpp"
#include "FunctionDefinition.hpp"
#include "VariableDefinition.hpp"
#include "TypeDefinition.hpp"
//#include "../compiler/IncorrectScriptParameterDefinitionException.hpp"
#include "NumberScriptParameter.hpp"
#include "MetadataString.hpp"
#include "MetadataIdentifier.hpp"
#include "../InternalErrorException.hpp"
#include "MetadataBoolean.hpp"
#include "BooleanScriptParameter.hpp"
#include "EnumScriptParameter.hpp"
#include "MetadataNumber.hpp"
#include "../corelib/EnumTypeDefinition.hpp"
#include "../compiler/ScriptParameterTypeNotSpecifiedException.hpp"
//#include "../compiler/IncorrectScriptParameterTypeException.hpp"
#include "../compiler/IncorrectScriptParameterAttributeTypeException.hpp"
#include "../compiler/IncorrectMetadataValueTypeException.hpp"
#include "../compiler/IncorrectScriptParameterValueRestrictionException.hpp"
#include "../compiler/UndefinedMetadataIdentifierException.hpp"
#include "../compiler/IncorrectMapSizeNumericValueException.hpp"
#include "../compiler/MinGreaterThanMaxSizeException.hpp"
#include "../compiler/IncorrectEnumScriptParameterTypeException.hpp"
#include "../compiler/ReservedScriptParameterNameException.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

const String CompiledScript::MAIN_FUNCTION_NAME = GG_STR("<main>");

CompiledScript::CompiledScript(String code) : code(code), metadata(CodeLocation(0, 0))
{
	this->AddLibrary(&this->coreLibrary);
//	this->metadata = NULL;
}

CompiledScript::~CompiledScript()
{
	for (std::vector<FunctionDefinition*>::iterator it = this->ownedFunctionDefinitions.begin(); it != this->ownedFunctionDefinitions.end(); it++)
	{
		delete *it;
	}

	for (std::vector<TypeDefinition*>::iterator it = this->ownedTypeDefinitions.begin(); it != this->ownedTypeDefinitions.end(); it++)
	{
		delete *it;
	}
}

bool CompiledScript::AddGlobalFunctionDefinition(FunctionDefinition* functionDefintion)
{
	if (!this->globalFunctionDefinitions.AddItem(functionDefintion))
	{
		return false;
	}

	this->ownedFunctionDefinitions.push_back(functionDefintion);

	return true;
}

bool CompiledScript::AddTypeDefinition(TypeDefinition* typeDefinition)
{
	if (!this->typeDefinitions.AddItem(typeDefinition))
	{
		return false;
	}

	this->ownedTypeDefinitions.push_back(typeDefinition);

	return true;
}

void CompiledScript::AddLibrary(Library const* library)
{
	for (SymbolDefinitionTable<TypeDefinition>::const_iterator it = library->GetTypeDefinitions().Begin(); it != library->GetTypeDefinitions().End(); it++)
	{
		if (!this->typeDefinitions.AddItem(it->second))
		{
			throw ApiUsageException(GG_STR("Type with the same name is already registered."));
		}
	}

	for (SymbolDefinitionTable<FunctionDefinition>::const_iterator it = library->GetGlobalFunctionDefinitions().Begin(); it != library->GetGlobalFunctionDefinitions().End(); it++)
	{
		if (!this->globalFunctionDefinitions.AddItem(it->second))
		{
			throw ApiUsageException(GG_STR("Global function with the same name is already registered."));
		}
	}

	for (SymbolDefinitionTable<VariableDefinition>::const_iterator it = library->GetGlobalVariableDefinitions().Begin(); it != library->GetGlobalVariableDefinitions().End(); it++)
	{
		if (!this->globalVariableDefinitions.AddItem(it->second))
		{
			throw ApiUsageException(GG_STR("Global variable with the same name is already registered."));
		}
	}
}

ScriptParameters CompiledScript::CreateScriptParameters() const
{
	unsigned defaultMapWidth = MAP_SIZE_AUTOMATIC;
	unsigned minMapWidth = MAP_SIZE_MIN;
	unsigned maxMapWidth = MAP_SIZE_MAX;	
	unsigned defaultMapHeight = MAP_SIZE_AUTOMATIC;
	unsigned minMapHeight = MAP_SIZE_MIN;
	unsigned maxMapHeight = MAP_SIZE_MAX;
	
	if (this->GetMetadata().ContainsItem(GG_STR("Width")))
	{	
		if (this->GetMetadata().GetItem(GG_STR("Width"))->GetType() == METADATA_TYPE_IDENTIFIER)
		{
			String identifierName = dynamic_cast<MetadataIdentifier const*>(this->GetMetadata().GetItem(GG_STR("Width")))->GetValue();
			if (identifierName == GG_STR("Infinite"))
			{
				defaultMapWidth = MAP_SIZE_INFINITE;
				minMapWidth = MAP_SIZE_INFINITE;
				maxMapWidth = MAP_SIZE_INFINITE;
			}
			else if (identifierName == GG_STR("Finite"))
			{
				defaultMapWidth = MAP_SIZE_AUTOMATIC;
				minMapWidth = MAP_SIZE_MIN;
				maxMapWidth = MAP_SIZE_MAX;
			}
			else
			{
				throw compiler::UndefinedMetadataIdentifierException(this->GetMetadata().GetItem(GG_STR("Width"))->GetLocation(), identifierName);
			}
		}		
		else if (this->GetMetadata().GetItem(GG_STR("Width"))->GetType() == METADATA_TYPE_KEYVALUE_COLLECTION)
		{
			MetadataKeyValueCollection const* widthSection = dynamic_cast<MetadataKeyValueCollection const*>(this->GetMetadata().GetItem(GG_STR("Width")));

			if (widthSection->ContainsItem(GG_STR("Default")))
			{
				if (widthSection->GetItem(GG_STR("Default"))->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(widthSection->GetItem(GG_STR("Default")))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(widthSection->GetItem(GG_STR("Default"))->GetLocation(), numericValue);
					}

					defaultMapWidth = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(widthSection->GetItem(GG_STR("Default"))->GetLocation(), GG_STR("Width"), GG_STR("Default"), MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(widthSection->GetItem(GG_STR("Default"))->GetType()));
				}
			}

			if (widthSection->ContainsItem(GG_STR("Min")))
			{
				if (widthSection->GetItem(GG_STR("Min"))->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(widthSection->GetItem(GG_STR("Min")))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(widthSection->GetItem(GG_STR("Min"))->GetLocation(), numericValue);
					}

					minMapWidth = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(widthSection->GetItem(GG_STR("Min"))->GetLocation(), GG_STR("Width"), GG_STR("Min"), MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(widthSection->GetItem(GG_STR("Min"))->GetType()));
				}
			}

			if (widthSection->ContainsItem(GG_STR("Max")))
			{
				if (widthSection->GetItem(GG_STR("Max"))->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(widthSection->GetItem(GG_STR("Max")))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(widthSection->GetItem(GG_STR("Max"))->GetLocation(), numericValue);
					}

					maxMapWidth = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(widthSection->GetItem(GG_STR("Max"))->GetLocation(), GG_STR("Width"), GG_STR("Max"), MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(widthSection->GetItem(GG_STR("Max"))->GetType()));
				}
			}

			if (minMapWidth > maxMapWidth)
			{
				throw compiler::MinGreaterThanMaxSizeException(widthSection->GetItem(GG_STR("Min"))->GetLocation(), DIRECTION_HORIZONTAL);
			}
		}
		else
		{
			throw compiler::IncorrectMetadataValueTypeException(GGE1408_WidthNotKeyValueCollection, this->GetMetadata().GetItem(GG_STR("Width"))->GetLocation(), GG_STR("Width"), MetadataTypeToString(METADATA_TYPE_KEYVALUE_COLLECTION), MetadataTypeToString(this->GetMetadata().GetItem(GG_STR("Width"))->GetType()));
		}
	}
	else
	{
		defaultMapWidth = MAP_SIZE_INFINITE;
		minMapWidth = MAP_SIZE_INFINITE;
		maxMapWidth = MAP_SIZE_INFINITE;
	}

	if (this->GetMetadata().ContainsItem(GG_STR("Height")))
	{
		if (this->GetMetadata().GetItem(GG_STR("Height"))->GetType() == METADATA_TYPE_IDENTIFIER)
		{
			String identifierName = dynamic_cast<MetadataIdentifier const*>(this->GetMetadata().GetItem(GG_STR("Height")))->GetValue();
			if (identifierName == GG_STR("Infinite"))
			{
				defaultMapHeight = MAP_SIZE_INFINITE;
				minMapHeight = MAP_SIZE_INFINITE;
				maxMapHeight = MAP_SIZE_INFINITE;
			}
			else if (identifierName == GG_STR("Finite"))
			{
				defaultMapHeight = MAP_SIZE_AUTOMATIC;
				minMapHeight = MAP_SIZE_MIN;
				maxMapHeight = MAP_SIZE_MAX;
			}
			else
			{
				throw compiler::UndefinedMetadataIdentifierException(this->GetMetadata().GetItem(GG_STR("Height"))->GetLocation(), identifierName);
			}
		}
		else if (this->GetMetadata().GetItem(GG_STR("Height"))->GetType() == METADATA_TYPE_KEYVALUE_COLLECTION)
		{
			MetadataKeyValueCollection const* heightSection = dynamic_cast<MetadataKeyValueCollection const*>(this->GetMetadata().GetItem(GG_STR("Height")));

			if (heightSection->ContainsItem(GG_STR("Default")))
			{
				if (heightSection->GetItem(GG_STR("Default"))->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(heightSection->GetItem(GG_STR("Default")))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(heightSection->GetItem(GG_STR("Default"))->GetLocation(), numericValue);
					}

					defaultMapHeight = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(heightSection->GetItem(GG_STR("Default"))->GetLocation(), GG_STR("Height"), GG_STR("Default"), MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(heightSection->GetItem(GG_STR("Default"))->GetType()));
				}
			}

			if (heightSection->ContainsItem(GG_STR("Min")))
			{
				if (heightSection->GetItem(GG_STR("Min"))->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(heightSection->GetItem(GG_STR("Min")))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(heightSection->GetItem(GG_STR("Min"))->GetLocation(), numericValue);
					}

					minMapHeight = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(heightSection->GetItem(GG_STR("Min"))->GetLocation(), GG_STR("Height"), GG_STR("Min"), MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(heightSection->GetItem(GG_STR("Min"))->GetType()));
				}
			}

			if (heightSection->ContainsItem(GG_STR("Max")))
			{
				if (heightSection->GetItem(GG_STR("Max"))->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(heightSection->GetItem(GG_STR("Max")))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(heightSection->GetItem(GG_STR("Max"))->GetLocation(), numericValue);
					}

					maxMapHeight = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(heightSection->GetItem(GG_STR("Max"))->GetLocation(), GG_STR("Height"), GG_STR("Max"), MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(heightSection->GetItem(GG_STR("Max"))->GetType()));
				}
			}

			if (minMapHeight > maxMapHeight)
			{
				throw compiler::MinGreaterThanMaxSizeException(heightSection->GetItem(GG_STR("Min"))->GetLocation(), DIRECTION_VERTICAL);
			}
		}
		else
		{
			throw compiler::IncorrectMetadataValueTypeException(GGE1409_HeightNotKeyValueCollection, this->GetMetadata().GetItem(GG_STR("Height"))->GetLocation(), GG_STR("Height"), MetadataTypeToString(METADATA_TYPE_KEYVALUE_COLLECTION), MetadataTypeToString(this->GetMetadata().GetItem(GG_STR("Height"))->GetType()));
		}
	}
	else
	{
		defaultMapHeight = MAP_SIZE_INFINITE;
		minMapHeight = MAP_SIZE_INFINITE;
		maxMapHeight = MAP_SIZE_INFINITE;
	}

	ScriptParameters table(defaultMapWidth, defaultMapHeight, minMapWidth, minMapHeight, maxMapWidth, maxMapHeight);

	// This code is first invoked by the compiler, so it throws compiler exceptions.
	if (this->GetMetadata().ContainsItem(GG_STR("Parameters")))
	{	
		if (this->GetMetadata().GetItem(GG_STR("Parameters"))->GetType() != METADATA_TYPE_KEYVALUE_COLLECTION)
		{
			throw compiler::IncorrectMetadataValueTypeException(GGE1402_ScriptParametersNotKeyValueCollection, this->GetMetadata().GetItem(GG_STR("Parameters"))->GetLocation(), GG_STR("Parameters"), MetadataTypeToString(METADATA_TYPE_KEYVALUE_COLLECTION), MetadataTypeToString(this->GetMetadata().GetItem(GG_STR("Parameters"))->GetType()));
		}

		MetadataKeyValueCollection const* parametersSection = dynamic_cast<MetadataKeyValueCollection const*>(this->GetMetadata().GetItem(GG_STR("Parameters")));

	
		for (MetadataKeyValueCollection::const_iterator it = parametersSection->Begin(); it != parametersSection->End(); it++)
		{		
			String name = it->first;
			CodeLocation location = it->second->GetLocation();
		
			if (it->second->GetType() != METADATA_TYPE_KEYVALUE_COLLECTION)
			{
				throw compiler::IncorrectMetadataValueTypeException(GGE1403_ScriptParameterNotKeyValueCollection, location, name, MetadataTypeToString(METADATA_TYPE_KEYVALUE_COLLECTION), MetadataTypeToString(this->GetMetadata().GetItem(GG_STR("Parameters"))->GetType()));
			}

			if (
				name == GG_STR("MapWidth") || 
				name == GG_STR("MapHeight") ||
				name == GG_STR("RenderOriginX") ||
				name == GG_STR("RenderOriginY") ||
				name == GG_STR("RenderWidth") ||
				name == GG_STR("RenderHeight") ||
				name == GG_STR("RenderScale"))
			{
				throw compiler::ReservedScriptParameterNameException(location, name);
			}

			MetadataKeyValueCollection const* currentSection = dynamic_cast<MetadataKeyValueCollection const*>(it->second);

			String label;
			if (currentSection->ContainsItem(GG_STR("Label")))
			{
				if (currentSection->GetItem(GG_STR("Label"))->GetType() != METADATA_TYPE_STRING)
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem(GG_STR("Label"))->GetLocation(), name, GG_STR("Label"), MetadataTypeToString(currentSection->GetItem(GG_STR("Label"))->GetType()), MetadataTypeToString(METADATA_TYPE_STRING));
				}

				label = dynamic_cast<MetadataString const*>(currentSection->GetItem(GG_STR("Label")))->GetValue();
			}
			else
			{
				label = name;
			}
		
			String description;
			if (currentSection->ContainsItem(GG_STR("Description")))
			{
				if (currentSection->GetItem(GG_STR("Description"))->GetType() != METADATA_TYPE_STRING)
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem(GG_STR("Description"))->GetLocation(), name, GG_STR("Description"), MetadataTypeToString(currentSection->GetItem(GG_STR("Description"))->GetType()), MetadataTypeToString(METADATA_TYPE_STRING));
				}

				description = dynamic_cast<MetadataString const*>(currentSection->GetItem(GG_STR("Description")))->GetValue();
			}

			ScriptParameterType parameterType = SCRIPT_PARAMETER_TYPE_NUMBER;
			if (currentSection->ContainsItem(GG_STR("Type")))
			{
				if (currentSection->GetItem(GG_STR("Type"))->GetType() != METADATA_TYPE_IDENTIFIER)
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem(GG_STR("Type"))->GetLocation(), name, GG_STR("Type"), MetadataTypeToString(currentSection->GetItem(GG_STR("Type"))->GetType()), MetadataTypeToString(METADATA_TYPE_IDENTIFIER));
				}

				String parameterTypeName = dynamic_cast<MetadataIdentifier const*>(currentSection->GetItem(GG_STR("Type")))->GetValue();
				if (parameterTypeName == GG_STR("Boolean"))
				{
					parameterType = SCRIPT_PARAMETER_TYPE_BOOLEAN;
				}
				else if (parameterTypeName == GG_STR("Number"))
				{
					parameterType = SCRIPT_PARAMETER_TYPE_NUMBER;
				}
				else
				{
					parameterType = SCRIPT_PARAMETER_TYPE_ENUM;
				}
			}
			else
			{
				bool typeDetermined = false;
				// First type inference rule - obtain the type from the default value.
				if (currentSection->ContainsItem(GG_STR("Default"))){
					MetadataType defaultType = currentSection->GetItem(GG_STR("Default"))->GetType();
					switch (defaultType)
					{
					case METADATA_TYPE_BOOLEAN:
						parameterType = SCRIPT_PARAMETER_TYPE_BOOLEAN;
						typeDetermined = true;
						break;
					case METADATA_TYPE_NUMBER:
						parameterType = SCRIPT_PARAMETER_TYPE_NUMBER;
						typeDetermined = true;
						break;
					default:
						break;
					}
				}

				// Second type inference rule - obtain the type from Min, Max and Restriction
				if (!typeDetermined){
					if (currentSection->ContainsItem(GG_STR("Min")) || currentSection->ContainsItem(GG_STR("Max")) || currentSection->ContainsItem(GG_STR("Restriction")))
					{
						parameterType = SCRIPT_PARAMETER_TYPE_NUMBER;
						typeDetermined = true;
					}
				}

				if(!typeDetermined) {
					throw compiler::ScriptParameterTypeNotSpecifiedException(location, name);
				}
			}

			ScriptParameter* parameter = NULL;
			switch (parameterType)
			{		
			case SCRIPT_PARAMETER_TYPE_BOOLEAN:
				{
					bool defaultValue = false;
					if (currentSection->ContainsItem(GG_STR("Default")))
					{
						if (currentSection->GetItem(GG_STR("Default"))->GetType() != METADATA_TYPE_BOOLEAN)
						{
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem(GG_STR("Default"))->GetLocation(), name, GG_STR("Default"), MetadataTypeToString(currentSection->GetItem(GG_STR("Default"))->GetType()), MetadataTypeToString(METADATA_TYPE_BOOLEAN));
						}

						defaultValue = dynamic_cast<MetadataBoolean const*>(currentSection->GetItem(GG_STR("Default")))->GetValue();
					}

					parameter = new BooleanScriptParameter(name, label, description, defaultValue);
					break;
				}
			case SCRIPT_PARAMETER_TYPE_NUMBER:
				{
					Number defaultValue = 0;
					if (currentSection->ContainsItem(GG_STR("Default")))
					{
						if (currentSection->GetItem(GG_STR("Default"))->GetType() != METADATA_TYPE_NUMBER)
						{
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem(GG_STR("Default"))->GetLocation(), name, GG_STR("Default"), MetadataTypeToString(currentSection->GetItem(GG_STR("Default"))->GetType()), MetadataTypeToString(METADATA_TYPE_NUMBER));
						}

						defaultValue = dynamic_cast<MetadataNumber const*>(currentSection->GetItem(GG_STR("Default")))->GetValue();
					}

					Number min = 0;
					if (currentSection->ContainsItem(GG_STR("Min")))
					{
						if (currentSection->GetItem(GG_STR("Min"))->GetType() != METADATA_TYPE_NUMBER)
						{
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem(GG_STR("Min"))->GetLocation(), name, GG_STR("Min"), MetadataTypeToString(currentSection->GetItem(GG_STR("Min"))->GetType()), MetadataTypeToString(METADATA_TYPE_NUMBER));
						}

						min = dynamic_cast<MetadataNumber const*>(currentSection->GetItem(GG_STR("Min")))->GetValue();
					}

					Number max = 100;
					if (currentSection->ContainsItem(GG_STR("Max")))
					{
						if (currentSection->GetItem(GG_STR("Max"))->GetType() != METADATA_TYPE_NUMBER)
						{
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem(GG_STR("Max"))->GetLocation(), name, GG_STR("Max"), MetadataTypeToString(currentSection->GetItem(GG_STR("Max"))->GetType()), MetadataTypeToString(METADATA_TYPE_NUMBER));
						}

						max = dynamic_cast<MetadataNumber const*>(currentSection->GetItem(GG_STR("Max")))->GetValue();
					}

					ScriptParameterValueRestriction restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED;
					if (currentSection->ContainsItem(GG_STR("Restriction")))
					{
						if (currentSection->GetItem(GG_STR("Restriction"))->GetType() != METADATA_TYPE_IDENTIFIER)
						{
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem(GG_STR("Restriction"))->GetLocation(), name, GG_STR("Restriction"), MetadataTypeToString(currentSection->GetItem(GG_STR("Restriction"))->GetType()), MetadataTypeToString(METADATA_TYPE_IDENTIFIER));
						}

						String restrictionName = dynamic_cast<MetadataIdentifier const*>(currentSection->GetItem(GG_STR("Restriction")))->GetValue();
						if (restrictionName == GG_STR("Unrestricted"))
						{
							restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED;
						}
						else if (restrictionName == GG_STR("Integers"))
						{
							restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_INTEGERS;
						}
						else if (restrictionName == GG_STR("PowersOf2"))
						{
							restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_2;
						}
						else if (restrictionName == GG_STR("PowersOf10"))
						{
							restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_10;
						}
						else
						{
							throw compiler::IncorrectScriptParameterValueRestrictionException(location, name, restrictionName);
						}
					}

					parameter = new NumberScriptParameter(name, label, description, defaultValue, min, max, restriction);
					break;
				}
			case SCRIPT_PARAMETER_TYPE_ENUM:
				{
					String enumTypeName = dynamic_cast<MetadataIdentifier const*>(currentSection->GetItem(GG_STR("Type")))->GetValue();

					TypeDefinition const* typeDefinition = this->GetTypeDefinitions().GetItem(enumTypeName);
					if (typeDefinition == NULL || !typeDefinition->IsEnumType()){
						throw compiler::IncorrectEnumScriptParameterTypeException(currentSection->GetItem(GG_STR("Type"))->GetLocation(), name, enumTypeName);
					}

					corelib::EnumTypeDefinition const* enumTypeDefinition = dynamic_cast<corelib::EnumTypeDefinition const*>(typeDefinition);

					int defaultValue;
					if (currentSection->ContainsItem(GG_STR("Default")))
					{
						if (currentSection->GetItem(GG_STR("Default"))->GetType() != METADATA_TYPE_IDENTIFIER)
						{
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem(GG_STR("Default"))->GetLocation(), name, GG_STR("Default"), MetadataTypeToString(currentSection->GetItem(GG_STR("Default"))->GetType()), MetadataTypeToString(METADATA_TYPE_IDENTIFIER));
						}

						String defaultValueName = dynamic_cast<MetadataIdentifier const*>(currentSection->GetItem(GG_STR("Default")))->GetValue();

						if (!enumTypeDefinition->IsValueStringDefined(defaultValueName))
						{
							throw compiler::UndefinedMetadataIdentifierException(currentSection->GetItem(GG_STR("Default"))->GetLocation(), defaultValueName);
						}

						defaultValue = enumTypeDefinition->GetValueDefinitions().find(defaultValueName)->second;
					}
					else 
					{
						defaultValue = enumTypeDefinition->GetDefaultValueInt();
					}

					parameter = new EnumScriptParameter(name, label, description, enumTypeDefinition, defaultValue);
					break;
				}
			default:
				throw InternalErrorException(GG_STR("Unknown parameter type."));
			}

			table.AddItem(name, parameter);
		}
	}
	return table;
}

void CompiledScript::Serialize(IOStream& stream) const
{
	stream << GG_STR("Code: ") << endl;
	stream << GG_STR("===============================") << endl;
	stream << this->code << endl;
	stream << GG_STR("===============================") << endl;

	stream << GG_STR("Configuration: ") << endl;
	this->configuration.SerializeWithTabs(stream, 1);

	stream << GG_STR("TypeDefinitions: ") << endl;
	this->GetTypeDefinitions().SerializeWithTabs(stream, 1);

	stream << GG_STR("GlobalFunctionDefinitions: ") << endl;
	this->GetGlobalFunctionDefinitions().SerializeWithTabs(stream, 1);

	stream << GG_STR("GlobalVariableDefinitions: ") << endl;
	this->GetGlobalVariableDefinitions().SerializeWithTabs(stream, 1);

	stream << GG_STR("Metadata: ") << endl;
	this->GetMetadata().SerializeWithTabs(stream, 1);

	stream << GG_STR("SupportedMaps: ") << endl;
	for (vector<String>::const_iterator it = this->GetSupportedMaps().begin(); it != this->GetSupportedMaps().end(); it++)
	{
		stream << "\t" << *it << endl;
	}
}