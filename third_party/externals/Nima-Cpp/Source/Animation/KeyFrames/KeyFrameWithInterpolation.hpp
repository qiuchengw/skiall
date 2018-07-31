#ifndef _NIMA_KEYFRAMEWITHINTERPOLATION_HPP_
#define _NIMA_KEYFRAMEWITHINTERPOLATION_HPP_

#include "KeyFrame.hpp"

namespace nima
{
	class KeyFrameWithInterpolation;

	class KeyFrameInterpolator
	{
		public:
			virtual ~KeyFrameInterpolator(){}
			virtual bool setNextFrame(KeyFrameWithInterpolation* frame, KeyFrame* nextFrame) = 0;
	};

	class KeyFrameWithInterpolation : public KeyFrame
	{
		typedef KeyFrame Base;

		protected:
			InterpolationType m_InterpolationType;
			KeyFrameInterpolator* m_Interpolator;

		public:
			KeyFrameWithInterpolation();
			virtual ~KeyFrameWithInterpolation();
			InterpolationType interpolationType() const;
			KeyFrameInterpolator* interpolator() const;

			bool read(BlockReader* reader, ActorComponent* component) override;
			void setNext(KeyFrame* frame) override;
	};
}

#endif