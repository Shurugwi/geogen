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

#include "HeightProfileNoiseFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileFlatRenderingStep.hpp"
#include "HeightProfileNoiseRenderingStep.hpp"
#include "ParseNoiseInput.hpp"
#include "../random/RandomSeed.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace renderer;
using namespace genlib;
using namespace random;

ManagedObject* HeightProfileNoiseFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Array")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(arrayTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 0);

	NoiseLayers layers = ParseNoiseInput(vm, location, arguments);

	RandomSeed argumentSeed = arguments.size() > 1 ? (RandomSeed)dynamic_cast<NumberObject*>(arguments[1])->GetValue() : 0;
	RandomSeed compositeSeed = CombineSeeds(argumentSeed, vm->GetArguments().GetRandomSeed());

	ManagedObject* returnObject = dynamic_cast<HeightProfileTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	// First create an empty height map
	unsigned objectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* flatRenderingStep = new HeightProfileFlatRenderingStep(location, vector<unsigned>(), objectSlot, 0);
	vm->AddRenderingStep(location, flatRenderingStep);

	// Then generate the noise layer by layer
	vector<unsigned> argumentSlots;
	argumentSlots.push_back(objectSlot);

	unsigned i = 0;
	for (NoiseLayers::const_iterator it = layers.begin(); it != layers.end(); it++)
	{
		RenderingStep* renderingStep = new HeightProfileNoiseRenderingStep(location, argumentSlots, objectSlot, it->first, it->second, compositeSeed, i);
		vm->AddRenderingStep(location, renderingStep);
		i++;
	}

	return returnObject;
}