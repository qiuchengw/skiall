#include "KeyFrameLength.hpp"
#include "../../ActorBone.hpp"

using namespace nima;

void KeyFrameLength::setValue(ActorComponent* component, float value, float mix)
{
	// Hard assumption this is a bone.
	ActorBone* bone = reinterpret_cast<ActorBone*>(component);
	bone->length(bone->length() * (1.0f - mix) + value * mix);
}