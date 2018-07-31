#include "ActorEvent.hpp"
#include "BlockReader.hpp"
using namespace nima;

ActorEvent::ActorEvent() : ActorComponent(ComponentType::ActorEvent)
{

}

ActorComponent* ActorEvent::makeInstance(Actor* resetActor)
{
	ActorEvent* eventInstance = new ActorEvent();
	eventInstance->copy(this, resetActor);
	return eventInstance;

}

void ActorEvent::copy(ActorComponent* node, Actor* resetActor)
{	
	Base::copy(node, resetActor);
}

ActorEvent* ActorEvent::read(Actor* actor, BlockReader* reader, ActorEvent* component)
{
	if(component == nullptr)
	{
		component = new ActorEvent();
	}

	Base::read(actor, reader, component);

	return component;
}