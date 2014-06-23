#include "HeightProfileFlatStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileFlatStep::Step(Renderer* renderer) const
{
	HeightProfile* profile = new HeightProfile(this->GetRenderOrigin(), this->GetRenderSize());
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_PROFILE, profile);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}