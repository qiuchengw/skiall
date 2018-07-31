#ifndef _NIMA_KEYFRAMEROTATION_HPP_
#define _NIMA_KEYFRAMEROTATION_HPP_

#include "KeyFrameNumeric.hpp"

namespace nima
{
	class ActorComponent;

	class KeyFrameRotation : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif