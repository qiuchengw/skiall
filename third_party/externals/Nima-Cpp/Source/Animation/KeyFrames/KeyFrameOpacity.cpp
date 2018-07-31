#include "KeyFrameOpacity.hpp"
#include "../../ActorNode.hpp"

using namespace nima;

void KeyFrameOpacity::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->opacity(node->opacity() * (1.0f - mix) + value * mix);
}