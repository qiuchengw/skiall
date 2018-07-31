#include "KeyFrameCustomProperty.hpp"
#include "../../BlockReader.hpp"
#include "../../CustomProperty.hpp"
#include <cmath>

using namespace nima;

void KeyFrameIntProperty::setValue(ActorComponent* component, float value, float mix)
{
	CustomIntProperty* property = reinterpret_cast<CustomIntProperty*>(component);
	property->value((int)round(property->value() * (1.0f - mix) + value * mix));
}

void KeyFrameFloatProperty::setValue(ActorComponent* component, float value, float mix)
{
	CustomFloatProperty* property = reinterpret_cast<CustomFloatProperty*>(component);
	property->value(property->value() * (1.0f - mix) + value * mix);
}

bool KeyFrameStringProperty::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}
	m_Value = reader->readString();
	return true;
}

void KeyFrameStringProperty::setNext(KeyFrame* frame)
{
	// Intentionally blank, we do not interpolate.
}

void KeyFrameStringProperty::apply(ActorComponent* component, float mix)
{
	CustomStringProperty* property = reinterpret_cast<CustomStringProperty*>(component);
	property->value(m_Value);
}

void KeyFrameStringProperty::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	apply(component, mix);
}

bool KeyFrameBooleanProperty::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}
	m_Value = reader->readByte() == 1;
	return true;
}

void KeyFrameBooleanProperty::setNext(KeyFrame* frame)
{
	// Intentionally blank, we do not interpolate.
}

void KeyFrameBooleanProperty::apply(ActorComponent* component, float mix)
{
	CustomBooleanProperty* property = reinterpret_cast<CustomBooleanProperty*>(component);
	property->value(m_Value);
}

void KeyFrameBooleanProperty::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	apply(component, mix);
}