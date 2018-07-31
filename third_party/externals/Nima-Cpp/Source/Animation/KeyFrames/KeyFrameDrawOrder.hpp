#ifndef _NIMA_KEYFRAMEDRAWORDER_HPP_
#define _NIMA_KEYFRAMEDRAWORDER_HPP_

#include "KeyFrame.hpp"

namespace nima
{
	class ActorComponent;

	class KeyFrameDrawOrder : public KeyFrame
	{
		typedef KeyFrame Base;
		private:
			struct DrawOrderIndex
			{
				unsigned short nodeIdx;
				unsigned short order;
			};
			DrawOrderIndex* m_OrderedNodes;
			int m_OrderedNodesCount;

		public:	
			KeyFrameDrawOrder();
			virtual ~KeyFrameDrawOrder();

			bool read(BlockReader* reader, ActorComponent* component) override;
			void setNext(KeyFrame* frame) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
	};
}

#endif