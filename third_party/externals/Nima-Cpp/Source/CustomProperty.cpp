#include "CustomProperty.hpp"
#include "BlockReader.hpp"
using namespace nima;


CustomIntProperty::CustomIntProperty() : ActorComponent(ComponentType::CustomIntProperty), m_Value(0)
{

}

ActorComponent* CustomIntProperty::makeInstance(Actor* resetActor)
{
	CustomIntProperty* instanceProp = new CustomIntProperty();
	instanceProp->copy(this, resetActor);
	return instanceProp;
}

void CustomIntProperty::copy(CustomIntProperty* property, Actor* resetActor)
{
	Base::copy(property, resetActor);
	m_Value = property->m_Value;
}

CustomIntProperty* CustomIntProperty::read(Actor* actor, BlockReader* reader, CustomIntProperty* property)
{
	if(property == nullptr)
	{
		property = new CustomIntProperty();
	}
	ActorComponent::read(actor, reader, property);
	property->m_Value = reader->readInt();
	return property;
}

void CustomIntProperty::resolveComponentIndices(ActorComponent** components)
{
	Base::resolveComponentIndices(components);

	ActorComponent* parent = components[parentIdx()];
	if(parent != nullptr)
	{
		parent->addCustomIntProperty(this);
	}
}

int CustomIntProperty::value() const
{
	return m_Value;
}

void CustomIntProperty::value(int v)
{
	m_Value = v;
}

CustomFloatProperty::CustomFloatProperty() : ActorComponent(ComponentType::CustomFloatProperty), m_Value(0.0f)
{

}

ActorComponent* CustomFloatProperty::makeInstance(Actor* resetActor)
{
	CustomFloatProperty* instanceProp = new CustomFloatProperty();
	instanceProp->copy(this, resetActor);
	return instanceProp;
}

void CustomFloatProperty::copy(CustomFloatProperty* property, Actor* resetActor)
{
	Base::copy(property, resetActor);
	m_Value = property->m_Value;
}

CustomFloatProperty* CustomFloatProperty::read(Actor* actor, BlockReader* reader, CustomFloatProperty* property)
{
	if(property == nullptr)
	{
		property = new CustomFloatProperty();
	}
	ActorComponent::read(actor, reader, property);
	property->m_Value = reader->readFloat();
	return property;
}

void CustomFloatProperty::resolveComponentIndices(ActorComponent** components)
{
	Base::resolveComponentIndices(components);
	ActorComponent* parent = components[parentIdx()];
	if(parent != nullptr)
	{
		parent->addCustomFloatProperty(this);
	}
}

float CustomFloatProperty::value() const
{
	return m_Value;
}

void CustomFloatProperty::value(float v)
{
	m_Value = v;
}

CustomStringProperty::CustomStringProperty() : ActorComponent(ComponentType::CustomStringProperty)
{

}

ActorComponent* CustomStringProperty::makeInstance(Actor* resetActor)
{
	CustomStringProperty* instanceProp = new CustomStringProperty();
	instanceProp->copy(this, resetActor);
	return instanceProp;
}

void CustomStringProperty::copy(CustomStringProperty* property, Actor* resetActor)
{
	Base::copy(property, resetActor);
	m_Value = property->m_Value;
}

CustomStringProperty* CustomStringProperty::read(Actor* actor, BlockReader* reader, CustomStringProperty* property)
{
	if(property == nullptr)
	{
		property = new CustomStringProperty();
	}
	ActorComponent::read(actor, reader, property);
	property->m_Value = reader->readString();
	return property;
}

void CustomStringProperty::resolveComponentIndices(ActorComponent** components)
{
	Base::resolveComponentIndices(components);

	ActorComponent* parent = components[parentIdx()];
	if(parent != nullptr)
	{
		parent->addCustomStringProperty(this);
	}
}

const std::string& CustomStringProperty::value() const
{
	return m_Value;
}

void CustomStringProperty::value(const std::string& v)
{
	m_Value = v;
}

CustomBooleanProperty::CustomBooleanProperty() : ActorComponent(ComponentType::CustomBooleanProperty)
{

}

ActorComponent* CustomBooleanProperty::makeInstance(Actor* resetActor)
{
	CustomBooleanProperty* instanceProp = new CustomBooleanProperty();
	instanceProp->copy(this, resetActor);
	return instanceProp;
}

void CustomBooleanProperty::copy(CustomBooleanProperty* property, Actor* resetActor)
{
	Base::copy(property, resetActor);
	m_Value = property->m_Value;
}

CustomBooleanProperty* CustomBooleanProperty::read(Actor* actor, BlockReader* reader, CustomBooleanProperty* property)
{
	if(property == nullptr)
	{
		property = new CustomBooleanProperty();
	}
	ActorComponent::read(actor, reader, property);
	property->m_Value = reader->readByte() == 1;
	return property;
}

void CustomBooleanProperty::resolveComponentIndices(ActorComponent** components)
{
	Base::resolveComponentIndices(components);

	ActorComponent* parent = components[parentIdx()];
	if(parent != nullptr)
	{
		parent->addCustomBooleanProperty(this);
	}
}

const bool CustomBooleanProperty::value() const
{
	return m_Value;
}

void CustomBooleanProperty::value(const bool v)
{
	m_Value = v;
}