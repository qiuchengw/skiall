#ifndef _NIMA_ACTORRENDERNODE_HPP_
#define _NIMA_ACTORRENDERNODE_HPP_

#include "ActorNode.hpp"
#include "BlendMode.hpp"

namespace nima
{
	class Actor;
	class BlockReader;
	class ActorNode;

	class ActorRenderNode : public ActorNode
	{
		typedef ActorNode Base;
		protected:
			int m_DrawOrder;
		public:
			ActorRenderNode(ComponentType type);
			void copy(ActorRenderNode* node, Actor* resetActor);
			int drawOrder() const;
			void drawOrder(int order);
	};
}
#endif