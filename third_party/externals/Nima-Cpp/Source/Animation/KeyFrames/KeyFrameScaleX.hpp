#ifndef _NIMA_KEYFRAMESCALEX_HPP_
#define _NIMA_KEYFRAMESCALEX_HPP_

#include "KeyFrameNumeric.hpp"

namespace nima
{
	class ActorComponent;

	class KeyFrameScaleX : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif