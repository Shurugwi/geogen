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
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class NonpositiveNoiseLayerWaveLengthException : public runtime::RuntimeException
		{
		private:
		public:
			NonpositiveNoiseLayerWaveLengthException(CodeLocation location) :
				RuntimeException(GGE2710_NonpositiveNoiseLayerWaveLength, location) {};

			virtual ~NonpositiveNoiseLayerWaveLengthException() throw () {}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Noise layer wave length was less than or equal to zero on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}