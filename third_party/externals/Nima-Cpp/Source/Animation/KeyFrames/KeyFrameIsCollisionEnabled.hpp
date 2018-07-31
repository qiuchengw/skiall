#ifndef _NIMA_KEYFRAMEISCOLLISIONENABLED_HPP_
#define _NIMA_KEYFRAMEISCOLLISIONENABLED_HPP_

#include "KeyFrameNumeric.hpp"
#include <string>

namespace nima
{
	class ActorComponent;

	class KeyFrameIsCollisionEnabled : public KeyFrame
	{
		typedef KeyFrame Base;
		private:
			bool m_Value;

		public:	
			bool read(BlockReader* reader, ActorComponent* component) override;
			void setNext(KeyFrame* frame) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
	};
}

#endif