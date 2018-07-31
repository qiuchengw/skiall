#include "KeyFrameTrigger.hpp"
#include "../../Actor.hpp"
#include "../../ActorImage.hpp"
#include "../../BlockReader.hpp"

using namespace nima;

			
KeyFrameTrigger::KeyFrameTrigger()
{

}

KeyFrameTrigger::~KeyFrameTrigger()
{
}


bool KeyFrameTrigger::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}
	
	return true;
}

void KeyFrameTrigger::setNext(KeyFrame* frame)
{
	// Do nothing, we don't interpolate.
}

void KeyFrameTrigger::apply(ActorComponent* component, float mix)
{
	// Intentionally blank.
}

void KeyFrameTrigger::applyInterpolation(ActorComponent* node, float time, KeyFrame* toFrame, float mix)
{
	// Do nothing, we don't interpolate.
}