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

#include "EnumScriptParameter.hpp"
#include "../InternalErrorException.hpp"
#include "../ApiUsageException.hpp"
#include "ManagedObject.hpp"
#include "../corelib/EnumTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;
using namespace geogen::corelib;

EnumScriptParameter::EnumScriptParameter(String const& name, String const& label, String const& description, EnumTypeDefinition const* enumType, int defaultValue)
: ScriptParameter(name, label, description), enumType(enumType)
{
	if (this->GetEnumType()->IsValueIntDefined(defaultValue))
	{
		this->defaultValue = this->value = defaultValue;
	}
	else {
		this->defaultValue = this->value = this->GetEnumType()->GetDefaultValueInt();
	}
}

bool EnumScriptParameter::EqualsTo(ScriptParameter const* other) const
{
	if (this->GetType() != other->GetType())
	{
		return false;
	}

	EnumScriptParameter const* typedOther = dynamic_cast<EnumScriptParameter const*>(other);

	return
		this->GetName() == other->GetName() &&
		this->GetLabel() == other->GetLabel() &&
		this->GetDescription() == other->GetDescription() &&
		this->GetDefaultValue() == typedOther->GetDefaultValue() &&
		this->GetEnumType() == typedOther->GetEnumType();
}

EnumScriptParameter::EnumScriptParameter(EnumScriptParameter const& other)
: ScriptParameter(other.name, other.label, other.description)
{
	this->enumType = other.enumType;
	this->defaultValue = other.defaultValue;
	this->value = other.GetValue();
}

EnumScriptParameter& EnumScriptParameter::operator=(EnumScriptParameter const& other)
{
	this->name = other.name;
	this->label = other.label;
	this->description = other.description;
	this->defaultValue = other.defaultValue;
	this->value = other.value;
	this->enumType = other.enumType;

	return *this;
}

void EnumScriptParameter::SetValue(int value)
{
	if (this->GetEnumType()->IsValueIntDefined(value))
	{
		this->value = value;
	}
	else {
		this->value = this->GetEnumType()->GetDefaultValueInt();
	}
}

void EnumScriptParameter::SetValue(String value)
{
	if (this->GetEnumType()->IsValueStringDefined(value))
	{
		this->value = this->GetEnumType()->GetValueDefinitions().find(value)->second;
	}
	else {
		this->value = this->GetEnumType()->GetDefaultValueInt();
	}
}

ScriptParameter* EnumScriptParameter::Clone() const
{
	return new EnumScriptParameter(*this);
}

void EnumScriptParameter::Serialize(IOStream& stream) const
{
	stream << GG_STR("Type: Enum") << endl;
	stream << GG_STR("Name: ") << this->name << endl;
	stream << GG_STR("Label: ") << this->label << endl;
	stream << GG_STR("Description: ") << this->description << endl;
	stream << GG_STR("DefaultValue: ") << this->defaultValue << endl;
	stream << GG_STR("Value: ") << this->value << endl;
	stream << GG_STR("EnumType: ") << this->enumType->GetName() << endl;
}