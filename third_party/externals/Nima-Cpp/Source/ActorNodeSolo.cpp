#include "ActorNodeSolo.hpp"
#include "BlockReader.hpp"

using namespace nima;

ActorNodeSolo::ActorNodeSolo() : 
	ActorNode(ComponentType::ActorNodeSolo),
	m_ActiveChildIndex(0)
{

}

int ActorNodeSolo::activeChildIndex() const
{
	return m_ActiveChildIndex;
}

void ActorNodeSolo::activeChildIndex(int index)
{
	if(m_ActiveChildIndex != index)
	{
		m_ActiveChildIndex = index;
		for(int i = 0; i < m_Children.size(); i++)
		{
			auto an = m_Children[i];
			bool cv = (i != (m_ActiveChildIndex-1));
			an->collapsedVisibility(cv);
		}
	}
}

ActorComponent* ActorNodeSolo::makeInstance(Actor* resetActor)
{
	ActorNodeSolo* soloInstance = new ActorNodeSolo();
	soloInstance->copy(this, resetActor);

	return soloInstance;
}

void ActorNodeSolo::copy(ActorNodeSolo* node, Actor* resetActor)
{
	Base::copy(node, resetActor);
	m_ActiveChildIndex = node->m_ActiveChildIndex;
}

ActorNodeSolo* ActorNodeSolo::read(Actor* actor, BlockReader* reader, ActorNodeSolo* node)
{
	if(node == nullptr)
	{
		node = new ActorNodeSolo();
	}

	Base::read(actor, reader, node);

	node->m_ActiveChildIndex = (int)reader->readFloat();

	return node;
}