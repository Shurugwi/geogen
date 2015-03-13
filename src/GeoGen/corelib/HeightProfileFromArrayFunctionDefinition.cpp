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

#include "HeightProfileFromArrayFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/IncorrectTypeException.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileFromArrayRenderingStep.hpp"
#include "DirectionTypeDefinition.hpp"
#include "HeightOverflowException.hpp"
#include "ArrayObject.hpp"
#include "CoordinateObject.hpp"
#include "UnknownRelativeCoordinateDirectionException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileFromArrayFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Array")));
	DirectionTypeDefinition const* directionTypeDefinition = dynamic_cast<DirectionTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Direction")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(arrayTypeDefinition);
	expectedTypes.push_back(directionTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	ArrayObject* arrayObject = (ArrayObject*)arguments[0];

	bool hasDirection = false;
	Direction direction = DIRECTION_HORIZONTAL;
	if (arguments.size() > 1)
	{
		direction = (Direction)NumberToInt(dynamic_cast<NumberObject*>(arguments[1])->GetValue());
		hasDirection = true;
	}

	map<Coordinate, Height> heights;
	for (ArrayObject::HashMap::iterator it = arrayObject->MBegin(); it != arrayObject->MEnd(); it++)
	{
		Coordinate key;
		if (it->first->GetType() == numberTypeDefinition)
		{
			// Todo: range check
			key = NumberToInt(dynamic_cast<NumberObject*>(it->first)->GetValue());
		}
		else if (it->first->GetType() == coordinateTypeDefinition)
		{
			CoordinateObject* coordinateObject = dynamic_cast<CoordinateObject*>(it->first);
			if (coordinateObject->IsRelative() && !hasDirection)
			{
				throw UnknownRelativeCoordinateDirectionException(location);
			}

			key = coordinateObject->GetAbsoluteCoordinate(vm, location, direction);
		}
		else IncorrectTypeException(GGE2706_IncorrectHeightProfileKeyType, location, GG_STR("Number"), it->second->GetType()->GetName());

		Height value;
		if (it->second->GetType() == numberTypeDefinition)
		{
			if (!TryNumberToHeight(dynamic_cast<NumberObject*>(it->second)->GetValue(), value))
			{
				throw HeightOverflowException(location);
			}
		}
		else IncorrectTypeException(GGE2707_IncorrectHeightProfileHeightType, location, GG_STR("Number"), it->second->GetType()->GetName());

		heights.insert(pair<Coordinate, Height>(key, value));
	}

	ManagedObject* returnObject = dynamic_cast<HeightProfileTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightProfileFromArrayRenderingStep(location, argumentSlots, returnObjectSlot, heights);
	vm->AddRenderingStep(location, renderingStep);

	return returnObject;
}