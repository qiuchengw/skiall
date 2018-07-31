#include "ActorAnimation.hpp"
#include "../ActorComponent.hpp"
#include "../BlockReader.hpp"
#include <algorithm>

using namespace nima;

ActorAnimation::ActorAnimation() :
			m_FPS(60),
			m_Duration(0.0f),
			m_IsLooping(false),
			m_AnimatedComponents(nullptr),
			m_AnimatedComponentsCount(0)
{

}

ActorAnimation::~ActorAnimation()
{
	delete [] m_AnimatedComponents;
}

const std::string& ActorAnimation::name() const
{
	return m_Name;
}


void ActorAnimation::duration(const float d)
{
	if(m_Duration != d)
	{
		m_Duration = d;
	}
}

float ActorAnimation::duration() const
{
	return m_Duration;
}

bool ActorAnimation::isLooping() const
{
	return m_IsLooping;
}

int ActorAnimation::fps() const
{
	return m_FPS;
}

void ActorAnimation::fps(int v)
{
	if (m_FPS != v)
	{
		m_FPS = v;
	}
}

void ActorAnimation::apply(float time, Actor* actor, float mix)
{
	// printf("TIME: %f\n", time);
	for(int i = 0; i < m_AnimatedComponentsCount; i++)
	{
		m_AnimatedComponents[i].apply(time, actor, mix);
	}
}

void ActorAnimation::triggerEvents(Actor* actor, float fromTime, float toTime, std::vector<ActorAnimationEvent>& events)
{
	for(auto keyedComponent : m_TriggerComponents)
	{
		keyedComponent->triggerEvents(actor, fromTime, toTime, events);
	}
}

void ActorAnimation::read(BlockReader* reader, ActorComponent** components)
{
	m_Name = reader->readString();
	m_FPS = (int)reader->readByte();
	m_Duration = reader->readFloat();
	m_IsLooping = reader->readByte() != 0;

	m_AnimatedComponentsCount = (int)reader->readUnsignedShort();
	m_AnimatedComponents = new ComponentAnimation[m_AnimatedComponentsCount];

	for(int i = 0; i < m_AnimatedComponentsCount; i++)
	{
		ComponentAnimation& animatedComponent = m_AnimatedComponents[i];
		animatedComponent.read(reader, components);

		// Put actor events in a separate list...
		ActorComponent* component = components[animatedComponent.componentIndex()];
		if(component != nullptr && component->type() == ComponentType::ActorEvent)
		{
			m_TriggerComponents.push_back(&animatedComponent);
		}
	}
}