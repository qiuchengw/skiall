#ifndef _NIMA_KEYFRAMEIKSTRENGTH_HPP_
#define _NIMA_KEYFRAMEIKSTRENGTH_HPP_

#include "KeyFrameNumeric.hpp"

namespace nima
{
	class ActorComponent;

	class KeyFrameIKStrength : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif