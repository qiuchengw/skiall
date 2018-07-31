#ifndef _NIMA_KEYFRAMESCALEY_HPP_
#define _NIMA_KEYFRAMESCALEY_HPP_

#include "KeyFrameNumeric.hpp"

namespace nima
{
	class ActorComponent;

	class KeyFrameScaleY : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif