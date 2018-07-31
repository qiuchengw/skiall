#include "ValueTimeCurveInterpolator.hpp"
#include "../../BinaryReader.hpp"
#include "../KeyFrames/KeyFrameNumeric.hpp"

using namespace nima;

ValueTimeCurveInterpolator::ValueTimeCurveInterpolator() :
			m_InFactor(0.0),
			m_InValue(0.0f),
			m_OutFactor(0.0),
			m_OutValue(0.0f)
{

}

bool ValueTimeCurveInterpolator::setNextFrame(KeyFrameWithInterpolation* frame, KeyFrame* nextFrame)
{
	// This frame is a hold, return false to remove the interpolator.
	// We store it in the first place as when it gets called as the nextFrame parameter (in a previous call)
	// we still read out the in factor and in value (see below where nextInValue and nextInFactor are defined).
	if(frame->interpolationType() == InterpolationType::Hold)
	{
		return false;
	}
	
	// Just a sanity check really, both keyframes need to be numeric.
	KeyFrameNumeric* ourFrame = reinterpret_cast<KeyFrameNumeric*>(frame);
	KeyFrameNumeric* next = reinterpret_cast<KeyFrameNumeric*>(nextFrame);
	// Should we change the above to dynamic and assert here?

	// We are not gauranteed to have a next interpolator (usually when the next keyframe is linear).
	ValueTimeCurveInterpolator* nextInterpolator = nullptr;

	float timeRange = next->time() - ourFrame->time();
	float outTime = (float)(ourFrame->time() + timeRange * m_OutFactor);

	float nextInValue = 0.0f;
	double nextInFactor = 0.0;

	// Get the invalue and infactor from the next interpolator (this is where hold keyframes get their interpolator values processed too).
	if((nextInterpolator = dynamic_cast<ValueTimeCurveInterpolator*>(next->interpolator())) != nullptr)
	{
		nextInValue = nextInterpolator->m_InValue;
		nextInFactor = nextInterpolator->m_InFactor;
	}
	else
	{
		// Happens when next is linear.
		nextInValue = next->value();
	}

	float inTime = (float)(next->time() - timeRange * nextInFactor);

	// Finally we can generate the curve.
	CubicSolver::set(ourFrame->time(), ourFrame->value(), outTime, m_OutValue, inTime, nextInValue, next->time(), next->value());

	return true;
}

bool ValueTimeCurveInterpolator::read(BinaryReader* reader, InterpolationType type)
{
	switch(type)
	{
		case InterpolationType::Mirrored:
		case InterpolationType::Asymmetric:
		case InterpolationType::Disconnected:
			m_InFactor = reader->readDouble();
			m_InValue = reader->readFloat();
			m_OutFactor = reader->readDouble();
			m_OutValue = reader->readFloat();
			return true;

		case InterpolationType::Hold:
			m_InFactor = reader->readDouble();
			m_InValue = reader->readFloat();
			m_OutFactor = 0.0;
			m_OutValue = 0.0f;
			return true;
		default:
			return false;
	}
}