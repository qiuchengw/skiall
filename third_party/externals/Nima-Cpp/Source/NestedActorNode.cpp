#include "NestedActorNode.hpp"
#include "BlockReader.hpp"
#include "Actor.hpp"

using namespace nima;

NestedActorNode::NestedActorNode() :
	ActorRenderNode(ComponentType::NestedActorNode),
	m_Asset(nullptr),
	m_ActorInstance(nullptr),
	m_TransformActorInstance(nullptr)
{

}

NestedActorNode::~NestedActorNode()
{
	delete m_ActorInstance;
}

ActorComponent* NestedActorNode::makeInstance(Actor* resetActor)
{
	NestedActorNode* instanceNode = new NestedActorNode();
	instanceNode->copy(this, resetActor);
	return instanceNode;
}

void NestedActorNode::setActorInstance(Actor* instance)
{
	m_ActorInstance = instance;
	m_TransformActorInstance = instance;
}

void NestedActorNode::transformNested(bool transformIt)
{
	m_TransformActorInstance = transformIt ? m_ActorInstance : nullptr;
}

void NestedActorNode::copy(NestedActorNode* node, Actor* resetActor)
{
	Base::copy(node, resetActor);

	m_DrawOrder = node->m_DrawOrder;
	m_Asset = node->m_Asset;
	Actor* actor = m_Asset->actor();
	if(actor != nullptr)
	{
		NestedActorNode* self = this;
		self->setActorInstance(m_Asset->actor()->makeInstance());
	}
}

void NestedActorNode::updateWorldTransform()
{
	Base::updateWorldTransform();
	if(m_TransformActorInstance != nullptr)
	{
		m_TransformActorInstance->root()->overrideWorldTransform(worldTransform());
	}
}

NestedActorNode* NestedActorNode::read(Actor* actor, BlockReader* reader, NestedActorNode* node)
{
	if (node == nullptr)
	{
		node = new NestedActorNode();
	}

	Base::read(actor, reader, node);

	bool isVisible = reader->readByte() != 0;
	if (isVisible)
	{
		node->m_DrawOrder = (int)reader->readUnsignedShort();
		int assetIndex = (int)reader->readUnsignedShort();
		node->m_Asset = actor->nestedActorAsset(assetIndex);
	}
	return node;
}

void NestedActorNode::advance(float elapsedSeconds)
{
	if(m_ActorInstance != nullptr)
	{
		m_ActorInstance->advance(elapsedSeconds);
	}
}