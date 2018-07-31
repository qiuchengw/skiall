#include "KeyFramePosX.hpp"
#include "../../ActorNode.hpp"

using namespace nima;

void KeyFramePosX::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->x(node->x() * (1.0f - mix) + value * mix);
}