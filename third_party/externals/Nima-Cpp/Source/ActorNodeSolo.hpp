#ifndef _NIMA_ACTORNODESOLO_HPP_
#define _NIMA_ACTORNODESOLO_HPP_

#include <string>
#include "ActorNode.hpp"

namespace nima
{
	class ActorNodeSolo : public ActorNode
	{
		typedef ActorNode Base;

		private:
			int m_ActiveChildIndex;

		public:
			ActorNodeSolo();
			int activeChildIndex() const;
			void activeChildIndex(int index);

			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorNodeSolo* node, Actor* resetActor);

			static ActorNodeSolo* read(Actor* actor, BlockReader* reader, ActorNodeSolo* node = NULL);
	};
}

#endif