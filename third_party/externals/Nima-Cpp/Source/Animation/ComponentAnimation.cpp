#include "ComponentAnimation.hpp"
#include "ActorAnimation.hpp"
#include "KeyFrames/KeyFrame.hpp"
#include "../Actor.hpp"
#include "../BlockReader.hpp"

using namespace nima;

ComponentAnimation::ComponentAnimation() :
	m_ComponentIndex(0),
	m_Properties(NULL),
	m_PropertiesCount(0)
{

}

ComponentAnimation::~ComponentAnimation()
{
	delete [] m_Properties;
}

int ComponentAnimation::componentIndex() const
{
	return m_ComponentIndex;
}

void ComponentAnimation::apply(float time, Actor* actor, float mix)
{
	ActorComponent* component = actor->component(m_ComponentIndex);
	for (int i = 0; i < m_PropertiesCount; i++)
	{
		m_Properties[i].apply(time, component, mix);
	}
}

void ComponentAnimation::triggerEvents(Actor* actor, float fromTime, float toTime, std::vector<ActorAnimationEvent>& events)
{
	for(int i = 0; i < m_PropertiesCount; i++)
	{
		PropertyAnimation& property = m_Properties[i];
		switch(property.type())
		{
			case PropertyType::Trigger:
			{
				int keyFramesCount = property.keyFramesCount();
				if(keyFramesCount == 0)
				{
					continue;
				}
				int idx = 0;
				// Binary find the keyframe index.
				{
					int mid = 0;
					float element = 0.0f;
					int start = 0;
					int end = keyFramesCount-1;

					while (start <= end) 
					{
				    	mid = ((start + end) >> 1);
						element = property.keyFrame(mid)->time();
						if (element < toTime) 
						{
							start = mid + 1;
						} 
						else if (element > toTime) 
						{
							end = mid - 1;
						} 
						else 
						{
							start = mid;
							break;
						}
					}

					idx = start;
				}

				if(idx == 0)
				{
					if(property.keyFrame(0)->time() == toTime)
					{
						ActorEvent* actorEvent = reinterpret_cast<ActorEvent*>(actor->component(m_ComponentIndex));
						events.emplace_back(ActorAnimationEvent(actorEvent, toTime, 0.0f));
						//ActorComponent component = components[keyedComponent.ComponentIndex];
						//triggerEvents.Add(new AnimationEventArgs(component.Name, component, property.PropertyType, toTime, 0.0f));
					}
				}
				else
				{
					for(int k = idx-1; k >= 0; k--)
					{
						float frameTime = property.keyFrame(k)->time();

						if(frameTime > fromTime)
						{
							ActorEvent* actorEvent = reinterpret_cast<ActorEvent*>(actor->component(m_ComponentIndex));
							events.emplace_back(ActorAnimationEvent(actorEvent, frameTime, toTime-frameTime));

							//ActorComponent component = components[keyedComponent.ComponentIndex];
							//triggerEvents.Add(new AnimationEventArgs(component.Name, component, property.PropertyType, frameTime, toTime-frameTime));
							/*triggered.push({
								name:component._Name,
								component:component,
								propertyType:property._Type,
								keyFrameTime:frame._Time,
								elapsed:toTime-frame._Time
							});*/
						}
						else
						{
							break;
						}
					}
				}
				break;
			}
			default:
				break;
		}	
	}
}

void ComponentAnimation::read(BlockReader* reader, ActorComponent** components)
{
	m_ComponentIndex = reader->readUnsignedShort();
	m_PropertiesCount = (int)reader->readUnsignedShort();
	m_Properties = new PropertyAnimation[m_PropertiesCount];
	ActorComponent* component = components[m_ComponentIndex];
	for (int i = 0; i < m_PropertiesCount; i++)
	{
		m_Properties[i].read(reader, component);
	}
}