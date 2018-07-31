#ifndef _NIMA_KEYFRAMEOPACITY_HPP_
#define _NIMA_KEYFRAMEOPACITY_HPP_

#include "KeyFrameNumeric.hpp"

namespace nima
{
	class ActorComponent;

	class KeyFrameOpacity : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif