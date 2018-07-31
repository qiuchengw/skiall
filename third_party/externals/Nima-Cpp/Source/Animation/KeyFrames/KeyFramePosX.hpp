#ifndef _NIMA_KEYFRAMEPOSX_HPP_
#define _NIMA_KEYFRAMEPOSX_HPP_

#include "KeyFrameNumeric.hpp"

namespace nima
{
	class ActorComponent;

	class KeyFramePosX : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif