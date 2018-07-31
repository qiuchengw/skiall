#ifndef _NIMA_VALUETIMECURVEINTERPOLATOR_HPP_
#define _NIMA_VALUETIMECURVEINTERPOLATOR_HPP_

#include "CubicSolver.hpp"
#include "../KeyFrames/KeyFrameWithInterpolation.hpp"

namespace nima
{	
	class BinaryReader;

	class ValueTimeCurveInterpolator : public CubicSolver, public KeyFrameInterpolator
	{
		private:
			double m_InFactor;
			float m_InValue;
			double m_OutFactor;
			float m_OutValue;

		public:
			ValueTimeCurveInterpolator();
			bool setNextFrame(KeyFrameWithInterpolation* frame, KeyFrame* nextFrame) override;
			bool read(BinaryReader* reader, InterpolationType type);
	
	};
}
#endif