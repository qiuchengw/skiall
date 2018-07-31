#include "KeyFrameRotation.hpp"
#include "../../ActorNode.hpp"

using namespace nima;

void KeyFrameRotation::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->rotation(node->rotation() * (1.0f - mix) + value * mix);
}