#include "KeyFrameWithInterpolation.hpp"
#include "../../BlockReader.hpp"
#include "../Interpolators/ValueTimeCurveInterpolator.hpp"

using namespace nima;

KeyFrameWithInterpolation::KeyFrameWithInterpolation() : 
			m_InterpolationType(InterpolationType::Hold),
			m_Interpolator(nullptr)
{

}

KeyFrameWithInterpolation::~KeyFrameWithInterpolation()
{
	delete m_Interpolator;
}

InterpolationType KeyFrameWithInterpolation::interpolationType() const
{
	return m_InterpolationType;
}

KeyFrameInterpolator* KeyFrameWithInterpolation::interpolator() const
{
	return m_Interpolator;
}

bool KeyFrameWithInterpolation::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}
	int type = (int)reader->readByte();

	if(type >= (int)InterpolationType::Max)
	{
		return false;
	}
	
	m_InterpolationType = static_cast<InterpolationType>(type);
	switch(m_InterpolationType)
	{
		case InterpolationType::Mirrored:
		case InterpolationType::Asymmetric:
		case InterpolationType::Disconnected:
		case InterpolationType::Hold:
		{
			ValueTimeCurveInterpolator* intr = new ValueTimeCurveInterpolator();
			m_Interpolator = intr;
			intr->read(reader, m_InterpolationType);
			break;
		}

		default:
			m_Interpolator = nullptr;
			break;

	}
	return true;
}

void KeyFrameWithInterpolation::setNext(KeyFrame* frame)
{
	// nullptr out the interpolator if the next frame doesn't validate.
	if(m_Interpolator != nullptr && !m_Interpolator->setNextFrame(this, frame))
	{
		delete m_Interpolator;
		m_Interpolator = nullptr;
	}
}