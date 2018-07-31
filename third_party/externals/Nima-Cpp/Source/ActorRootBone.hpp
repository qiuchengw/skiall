#ifndef _NIMA_ACTORROOTBONE_HPP_
#define _NIMA_ACTORROOTBONE_HPP_

#include "ActorNode.hpp"

namespace nima
{
	class Actor;
	class BlockReader;
	class ActorNode;

	class ActorRootBone : public ActorNode
	{
		public:
			ActorRootBone();
			ActorNode* makeInstance(Actor* resetActor);
			void copy(ActorRootBone* node, Actor* resetActor);

			static ActorRootBone* read(Actor* actor, BlockReader* reader, ActorRootBone* node = NULL);
	};
}
#endif