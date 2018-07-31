#include "ActorRenderNode.hpp"
#include "Actor.hpp"

using namespace nima;

ActorRenderNode::ActorRenderNode(ComponentType type) :
	ActorNode(type),
	m_DrawOrder(0)
{

}

void ActorRenderNode::copy(ActorRenderNode* node, Actor* resetActor)
{
	Base::copy(node, resetActor);

	m_DrawOrder = node->m_DrawOrder;
}

int ActorRenderNode::drawOrder() const
{
	return m_DrawOrder;
}

void ActorRenderNode::drawOrder(int order)
{
	if (m_DrawOrder != order)
	{
		m_DrawOrder = order;
		if(m_Actor != nullptr)
		{
			m_Actor->markDrawOrderDirty();
		}
	}
}