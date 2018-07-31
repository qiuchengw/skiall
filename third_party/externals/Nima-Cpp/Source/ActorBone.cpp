#include "ActorBone.hpp"
#include "BlockReader.hpp"

using namespace nima;

ActorBone::ActorBone() : ActorNode(ComponentType::ActorBone), m_Length(0.0f), m_IsConnectedToImage(false)
{
}

float ActorBone::length() const
{
	return m_Length;
}

void ActorBone::length(float l)
{
	if(m_Length == l)
	{
		return;
	}
	m_Length = l;
	for(ActorNode* node : m_Children)
	{
		if(node->type() == ComponentType::ActorBone)
		{
			ActorBone* bone = reinterpret_cast<ActorBone*>(node);
			bone->x(l);
		}
	}
}

bool ActorBone::isConnectedToImage() const
{
	return m_IsConnectedToImage;
}

void ActorBone::isConnectedToImage(bool isIt)
{
	m_IsConnectedToImage = isIt;
}

Vec2D ActorBone::tipWorldTranslation()
{
	Mat2D transform;
	transform[4] = m_Length;

	Mat2D::multiply(transform, worldTransform(), transform);

	Vec2D result;
	result[0] = transform[4];
	result[1] = transform[5];

	return result;
}

void ActorBone::tipWorldTranslation(Vec2D& result)
{
	Mat2D transform;
	transform[4] = m_Length;

	Mat2D::multiply(transform, worldTransform(), transform);
	result[0] = transform[4];
	result[1] = transform[5];
}

ActorBone* ActorBone::read(Actor* actor, BlockReader* reader, ActorBone* node)
{
	if(node == nullptr)
	{
		node = new ActorBone();
	}

	ActorNode::read(actor, reader, node);

	node->m_Length = reader->readFloat();

	return node;
}

ActorComponent* ActorBone::makeInstance(Actor* resetActor)
{
	ActorBone* instanceNode = new ActorBone();
	instanceNode->copy(this, resetActor);
	return instanceNode;
}

void ActorBone::copy(ActorBone* node, Actor* resetActor)
{
	ActorNode::copy(node, resetActor);
	m_Length = node->m_Length;
	m_IsConnectedToImage = node->m_IsConnectedToImage;
}
