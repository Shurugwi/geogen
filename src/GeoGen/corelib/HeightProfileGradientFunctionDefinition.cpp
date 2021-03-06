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

#include "HeightProfileGradientFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "CoordinateObject.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileGradientRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "UnknownRelativeCoordinateDirectionException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileGradientFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(coordinateTypeDefinition);
	expectedTypes.push_back(coordinateTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	CoordinateObject* sourceObject = dynamic_cast<CoordinateObject*>(arguments[0]);
	CoordinateObject* destinationObject = dynamic_cast<CoordinateObject*>(arguments[1]);

	if (sourceObject->IsRelative() || destinationObject->IsRelative())
	{
		throw UnknownRelativeCoordinateDirectionException(location);
	}

	Coordinate source = (Coordinate)sourceObject->GetValue();
	Coordinate destination = (Coordinate)destinationObject->GetValue();

	Number numberFromHeight = ((NumberObject*)arguments[2])->GetValue();
	Height fromHeight;
	if (!TryNumberToHeight(numberFromHeight, fromHeight))
	{
		throw HeightOverflowException(location);
	}

	Number numberToHeight = ((NumberObject*)arguments[3])->GetValue();
	Height toHeight;
	if (!TryNumberToHeight(numberToHeight, toHeight))
	{
		throw HeightOverflowException(location);
	}

	ManagedObject* returnObject = dynamic_cast<HeightProfileTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightProfileGradientRenderingStep(location, argumentSlots, returnObjectSlot, source, destination, fromHeight, toHeight);
	vm->AddRenderingStep(location, renderingStep);

	return returnObject;
}