#ifndef _NIMA_NESTEDACTORNODE_HPP_
#define _NIMA_NESTEDACTORNODE_HPP_

#include "ActorRenderNode.hpp"

namespace nima
{
	class Actor;
	class BlockReader;
	class NestedActorAsset;

	class NestedActorNode : public ActorRenderNode
	{
		typedef ActorRenderNode Base;
		private:
			NestedActorAsset* m_Asset;
			Actor* m_ActorInstance;
			Actor* m_TransformActorInstance;

		protected:

			virtual void setActorInstance(Actor* instance);

		public:
			NestedActorNode();
			virtual ~NestedActorNode();

			ActorComponent* makeInstance(Actor* resetActor) override;
			void updateWorldTransform() override;
			void copy(NestedActorNode* node, Actor* resetActor);
			void advance(float elapsedSeconds);
			void transformNested(bool transformIt);

			static NestedActorNode* read(Actor* actor, BlockReader* reader, NestedActorNode* node = NULL);
	};
}
#endif