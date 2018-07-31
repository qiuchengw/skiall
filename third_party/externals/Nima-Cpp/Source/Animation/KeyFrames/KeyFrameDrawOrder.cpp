#include "KeyFrameDrawOrder.hpp"
#include "../../Actor.hpp"
#include "../../ActorRenderNode.hpp"
#include "../../BlockReader.hpp"

using namespace nima;

			
KeyFrameDrawOrder::KeyFrameDrawOrder() :
	m_OrderedNodes(NULL),
	m_OrderedNodesCount(0)
{

}

KeyFrameDrawOrder::~KeyFrameDrawOrder()
{
	delete m_OrderedNodes;
}


bool KeyFrameDrawOrder::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}
	m_OrderedNodesCount = (int)reader->readUnsignedShort();
	m_OrderedNodes = new DrawOrderIndex[m_OrderedNodesCount];
	for(int i = 0; i < m_OrderedNodesCount; i++)
	{
		DrawOrderIndex& drawOrder = m_OrderedNodes[i];
		drawOrder.nodeIdx = reader->readUnsignedShort();
		drawOrder.order = reader->readUnsignedShort();
	}
	return true;
}

void KeyFrameDrawOrder::setNext(KeyFrame* frame)
{
	// Do nothing, we don't interpolate.
}

void KeyFrameDrawOrder::apply(ActorComponent* component, float mix)
{
	Actor* actor = component->actor();

	for(int i = 0; i < m_OrderedNodesCount; i++)
	{
		DrawOrderIndex& doi = m_OrderedNodes[i];
		// Some pretty hard assumptions being made here. We're assuming we had good data to begin with.
		// Could validate it at load time by passing the actor into the read methods.
		ActorRenderNode* renderNode = dynamic_cast<ActorRenderNode*>(actor->component(doi.nodeIdx));
		if(renderNode != nullptr)
		{
			renderNode->drawOrder(doi.order);
		}
	}
}

void KeyFrameDrawOrder::applyInterpolation(ActorComponent* node, float time, KeyFrame* toFrame, float mix)
{
	apply(node, mix);
}