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

#include "../../Number.hpp"
#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadConstNumberInstruction : public Instruction
			{
			private:
				Number constNumber;
			public:				
				LoadConstNumberInstruction(CodeLocation location, Number constNumber) : Instruction(location)
				{
					this->constNumber = constNumber;
				}

				virtual void Serialize(IOStream& stream) const { stream << "LoadConstNumber " << constNumber; }

				virtual String GetInstructionName() const { return GG_STR("LoadConstNumber"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const { return new LoadConstNumberInstruction(this->GetLocation(), this->constNumber); };
			};
		}
	}
}