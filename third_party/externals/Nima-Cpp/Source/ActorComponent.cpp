#include "ActorComponent.hpp"
#include "ActorNode.hpp"
#include "CustomProperty.hpp"
#include "BlockReader.hpp"
#include <algorithm>
#include <cassert>
using namespace nima;

ActorComponent::ActorComponent(ComponentType type) : ActorComponent(nullptr, type)
{

}

ActorComponent::ActorComponent(Actor* actor, ComponentType type) : 
			m_Type(type),
			m_Parent(nullptr),
			m_Actor(actor),
			m_ParentIdx(0),
			m_Idx(0)
{

}

ActorComponent::~ActorComponent()
{

}

Actor* ActorComponent::actor() const
{
	return m_Actor;
}

const std::string& ActorComponent::name() const
{
	return m_Name;
}

ComponentType ActorComponent::type() const
{
	return m_Type;
}

ActorNode* ActorComponent::parent() const
{
	return m_Parent;	
}

unsigned short ActorComponent::parentIdx() const
{
	return m_ParentIdx;
}

unsigned short ActorComponent::idx() const
{
		return m_Idx;
}

void ActorComponent::resolveComponentIndices(ActorComponent** components)
{
	ActorComponent* component = components[m_ParentIdx];
	if(component != nullptr && component->isNode())
	{
		m_Parent = static_cast<ActorNode*>(component);
		if(this->isNode())
		{
			m_Parent->addChild(static_cast<ActorNode*>(this));
		}
	}
}

void ActorComponent::copy(ActorComponent* component, Actor* resetActor)
{
	m_Name = component->m_Name;
	m_Actor = resetActor;
	m_ParentIdx = component->m_ParentIdx;
	m_Idx = component->m_Idx;
}

ActorComponent* ActorComponent::read(Actor* actor, BlockReader* reader, ActorComponent* component)
{
	component->m_Actor = actor;
	component->m_Name = reader->readString();
	component->m_ParentIdx = reader->readUnsignedShort();

	return component;
}

void ActorComponent::addCustomIntProperty(CustomIntProperty* property)
{
	m_CustomIntProperties.push_back(property);
}

void ActorComponent::addCustomFloatProperty(CustomFloatProperty* property)
{
	m_CustomFloatProperties.push_back(property);
}

void ActorComponent::addCustomStringProperty(CustomStringProperty* property)
{
	m_CustomStringProperties.push_back(property);
}

void ActorComponent::addCustomBooleanProperty(CustomBooleanProperty* property)
{
	m_CustomBooleanProperties.push_back(property);
}

CustomIntProperty* ActorComponent::getCustomIntProperty(const std::string& name)
{
	for(CustomIntProperty* prop : m_CustomIntProperties)
	{
		if(prop->name() == name)
		{
			return prop;
		}
	}
	return nullptr;
}

CustomFloatProperty* ActorComponent::getCustomFloatProperty(const std::string& name)
{
	for(CustomFloatProperty* prop : m_CustomFloatProperties)
	{
		if(prop->name() == name)
		{
			return prop;
		}
	}
	return nullptr;
}

CustomStringProperty* ActorComponent::getCustomStringProperty(const std::string& name)
{
	for(CustomStringProperty* prop : m_CustomStringProperties)
	{
		if(prop->name() == name)
		{
			return prop;
		}
	}
	return nullptr;
}

CustomBooleanProperty* ActorComponent::getCustomBooleanProperty(const std::string& name)
{
	for(CustomBooleanProperty* prop : m_CustomBooleanProperties)
	{
		if(prop->name() == name)
		{
			return prop;
		}
	}
	return nullptr;
}