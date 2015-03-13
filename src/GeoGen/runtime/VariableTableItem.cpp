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
#include "ManagedObject.hpp"

using namespace std;
using namespace geogen::runtime;

bool VariableTableItem::SetValue(VirtualMachine* vm, ManagedObject* value)
{
	if (this->isConst)
	{
		return false;
	}

	if (value != this->value)
	{
		this->value->RemoveRef(vm->GetMemoryManager());
		value->AddRef();

		this->value = value;
	}

	return true;
}