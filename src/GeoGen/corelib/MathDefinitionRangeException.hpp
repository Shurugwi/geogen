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

#include "../String.hpp"
#include "../Number.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class MathDefinitionRangeException : public runtime::RuntimeException
		{
		private:
			String operationName;
			Number number;
		public:
			MathDefinitionRangeException(ErrorCode errorCode, CodeLocation location, String const& operationName, Number number) :
				RuntimeException(errorCode, location), operationName(operationName), number(number) {};

			virtual ~MathDefinitionRangeException() throw () {}

			inline Number GetNumber()
			{
				return this->number;
			}

			inline String GetOperationName()
			{
				return this->operationName;
			}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "\"" << this->GetNumber() << "\" is outside of the definition range of operation \"" << this->GetOperationName() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}