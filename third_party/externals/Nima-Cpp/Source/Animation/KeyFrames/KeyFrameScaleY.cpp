#include "KeyFrameScaleY.hpp"
#include "../../ActorNode.hpp"

using namespace nima;

void KeyFrameScaleY::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->scaleY(node->scaleY() * (1.0f - mix) + value * mix);
}