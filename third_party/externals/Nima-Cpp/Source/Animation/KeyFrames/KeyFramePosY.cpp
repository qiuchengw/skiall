#include "KeyFramePosY.hpp"
#include "../../ActorNode.hpp"

using namespace nima;

void KeyFramePosY::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->y(node->y() * (1.0f - mix) + value * mix);
}