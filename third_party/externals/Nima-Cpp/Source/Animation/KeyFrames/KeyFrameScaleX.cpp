#include "KeyFrameScaleX.hpp"
#include "../../ActorNode.hpp"

using namespace nima;

void KeyFrameScaleX::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->scaleX(node->scaleX() * (1.0f - mix) + value * mix);
}