#include "KeyFrameNumeric.hpp"
#include "../Interpolators/ValueTimeCurveInterpolator.hpp"
#include "../../BlockReader.hpp"

using namespace nima;

KeyFrameNumeric::KeyFrameNumeric() : 
	m_Value(0.0f)
{

}

float KeyFrameNumeric::value() const
{
	return m_Value;
}

bool KeyFrameNumeric::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}

	m_Value = reader->readFloat();

	return true;
}

void KeyFrameNumeric::apply(ActorComponent* component, float mix)
{
	this->setValue(component, m_Value, mix);
}

void KeyFrameNumeric::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	switch(m_InterpolationType)
	{
		case InterpolationType::Mirrored:
		case InterpolationType::Asymmetric:
		case InterpolationType::Disconnected:
		{
			ValueTimeCurveInterpolator* interpolator = reinterpret_cast<ValueTimeCurveInterpolator*>(m_Interpolator);
			if(interpolator != nullptr)
			{
				float v = (float)interpolator->get((double)time);
				setValue(component, v, mix);
			}
			break;
		}

		case InterpolationType::Hold:
		{
			setValue(component, m_Value, mix);
			break;
		}

		case InterpolationType::Linear:
		{
			KeyFrameNumeric* to = reinterpret_cast<KeyFrameNumeric*>(toFrame);

			float f = (time - m_Time)/(to->m_Time-m_Time);
			setValue(component, m_Value * (1.0f-f) + to->m_Value * f, mix);
			break;
		}

		default:
			// Unhandled interpolation...
			break;
	}
}

KeyFrameInt::KeyFrameInt() : 
	m_Value(0)
{

}

int KeyFrameInt::value() const
{
	return m_Value;
}

bool KeyFrameInt::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}

	m_Value = reader->readInt();

	return true;
}

void KeyFrameInt::apply(ActorComponent* component, float mix)
{
	this->setValue(component, m_Value, mix);
}

void KeyFrameInt::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	switch(m_InterpolationType)
	{
		case InterpolationType::Mirrored:
		case InterpolationType::Asymmetric:
		case InterpolationType::Disconnected:
		{
			ValueTimeCurveInterpolator* interpolator = reinterpret_cast<ValueTimeCurveInterpolator*>(m_Interpolator);
			if(interpolator != nullptr)
			{
				float v = (float)interpolator->get((double)time);
				setValue(component, v, mix);
			}
			break;
		}

		case InterpolationType::Hold:
		{
			setValue(component, m_Value, mix);
			break;
		}

		case InterpolationType::Linear:
		{
			KeyFrameInt* to = reinterpret_cast<KeyFrameInt*>(toFrame);

			float f = (time - m_Time)/(to->m_Time-m_Time);
			setValue(component, m_Value * (1.0f-f) + to->m_Value * f, mix);
			break;
		}

		default:
			// Unhandled interpolation...
			break;
	}
}