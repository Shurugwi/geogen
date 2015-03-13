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

#include "ArrayShuffleFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../random/RandomSeed.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::random;

ManagedObject* ArrayShuffleFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 0);

	RandomSeed seed = arguments.size() > 0 ? (RandomSeed)NumberToInt(dynamic_cast<NumberObject*>(arguments[0])->GetValue()) : (RandomSeed)vm->GetCommonRandomSequence().NextUInt();

	RandomSeed finalSeed = CombineSeeds(vm->GetArguments().GetRandomSeed(), seed, CreateSeed(GG_STR("Array.Shuffle")));

	ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

	thisArray->Shuffle(vm, location, finalSeed);

	return vm->GetNull();
}