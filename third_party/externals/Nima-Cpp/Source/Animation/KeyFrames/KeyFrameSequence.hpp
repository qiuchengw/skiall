#ifndef _NIMA_KEYFRAMESEQUENCE_HPP_
#define _NIMA_KEYFRAMESEQUENCE_HPP_

#include "KeyFrameNumeric.hpp"

namespace nima
{
	class KeyFrameSequence : public KeyFrameNumeric
	{
		public:
            bool read(BlockReader* reader, ActorComponent* component) override;
            void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif