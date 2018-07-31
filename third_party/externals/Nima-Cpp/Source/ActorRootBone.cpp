#include "ActorRootBone.hpp"

using namespace nima;

ActorRootBone::ActorRootBone() : ActorNode(ComponentType::ActorRootBone)
{
}

ActorNode* ActorRootBone::makeInstance(Actor* resetActor)
{
	ActorRootBone* instanceNode = new ActorRootBone();
	instanceNode->copy(this, resetActor);
	return instanceNode;
}

void ActorRootBone::copy(ActorRootBone* node, Actor* resetActor)
{
	ActorNode::copy(node, resetActor);
}

ActorRootBone* ActorRootBone::read(Actor* actor, BlockReader* reader, ActorRootBone* node)
{
	if (node == nullptr)
	{
		node = new ActorRootBone();
	}

	ActorNode::read(actor, reader, node);

	return node;
}