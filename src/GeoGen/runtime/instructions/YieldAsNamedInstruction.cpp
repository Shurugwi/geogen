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

#include "YieldAsNamedInstruction.hpp"
#include "../CodeBlockStackEntry.hpp"
#include "../VirtualMachine.hpp"
#include "../../InternalErrorException.hpp"
#include "../ManagedObject.hpp"
#include "../../corelib/YieldRenderingStep.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;
using namespace geogen::corelib;
using namespace geogen::renderer;

InstructionStepResult YieldAsNamedInstruction::Step(VirtualMachine* vm) const
{
	vm->GetObjectStack().CheckSize(1);
	ManagedObject* yieldedValue = vm->GetObjectStack().Top();
	yieldedValue->AddRef();
	vm->GetObjectStack().Pop(vm);

	if (vm->GetArguments().GetEnabledMaps().empty() || find(vm->GetArguments().GetEnabledMaps().begin(), vm->GetArguments().GetEnabledMaps().end(), this->functionName) != vm->GetArguments().GetEnabledMaps().end())
	{
		vector<unsigned> argumentSlots;
		argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(yieldedValue));
		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(yieldedValue);

		Coordinate renderOriginX = vm->GetArguments().GetRenderOriginX();
		Size1D renderWidth = vm->GetArguments().GetRenderWidth();
		if (renderWidth == MAP_SIZE_AUTOMATIC)
		{
			if (vm->GetArguments().GetMaxMapWidth() != MAP_SIZE_INFINITE)
			{
				renderWidth = vm->GetArguments().GetMapWidth();
			}
			else 
			{
				renderWidth = RENDER_SIZE_DEFAULT;
			}
		}

		if (vm->GetArguments().GetMaxMapWidth() != MAP_SIZE_INFINITE)
		{
			renderOriginX = max(renderOriginX, 0);
			renderWidth = min(Size1D(vm->GetArguments().GetMapWidth()/* * vm->GetArguments().GetRenderScale()*/) - renderOriginX, renderWidth);
		}

		Coordinate renderOriginY = vm->GetArguments().GetRenderOriginY();
		Size1D renderHeight = vm->GetArguments().GetRenderHeight();
		if (renderHeight == MAP_SIZE_AUTOMATIC)
		{
			if (vm->GetArguments().GetMaxMapHeight() != MAP_SIZE_INFINITE)
			{
				renderHeight = vm->GetArguments().GetMapHeight();
			}
			else
			{
				renderHeight = RENDER_SIZE_DEFAULT;
			}
		}

		if (vm->GetArguments().GetMaxMapHeight() != MAP_SIZE_INFINITE)
		{
			renderOriginY = max(renderOriginY, 0);
			renderHeight = min(Size1D(vm->GetArguments().GetMapHeight() /** vm->GetArguments().GetRenderScale()*/) - renderOriginY, renderHeight);
		}

		//Rectangle mapRectangle = Rectangle(Point(0, 0), Size2D(vm->GetArguments().GetMapWidth(), vm->GetArguments().GetMapHeight()));
		Rectangle renderRectangle = Rectangle(Point(vm->GetArguments().GetRenderOriginX(), vm->GetArguments().GetRenderOriginY()), Size2D(renderWidth, renderHeight));
		//Rectangle actualRenderRectangle = Rectangle::Intersect(mapRectangle, renderRectangle);

		RenderingStep* renderingStep = new YieldRenderingStep(this->GetLocation(), argumentSlots, returnObjectSlot, this->functionName, renderRectangle);
		vm->AddRenderingStep(this->GetLocation(), renderingStep);
	}

	yieldedValue->RemoveRef(vm->GetMemoryManager());

	vm->GetGeneratedMaps().push_back(this->functionName);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}