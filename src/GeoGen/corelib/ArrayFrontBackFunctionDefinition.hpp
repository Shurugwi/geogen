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

#pragma once

#include <vector>

#include "../String.hpp"
#include "../runtime/MemberNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"
#include "ArrayTypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		/// Definition for functions Array.Front and Array.Back.
		class ArrayFrontBackFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			enum Method
			{
				FRONT,
				BACK
			};
		private:
			Method method;

			ArrayFrontBackFunctionDefinition(String const& name, Method method, runtime::TypeDefinition const* owningType) : MemberNativeFunctionDefinition(name, owningType), method(method) {};
		public:
			static ArrayFrontBackFunctionDefinition* Create(Method method, runtime::TypeDefinition const* owningType);

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual runtime::MethodType GetMethodType() const { return runtime::METHOD_TYPE_INSTANCE; };
		};
	}
}