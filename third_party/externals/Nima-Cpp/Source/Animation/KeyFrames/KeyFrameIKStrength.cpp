#include "KeyFrameIKStrength.hpp"
#include "../../ActorIKTarget.hpp"

using namespace nima;

void KeyFrameIKStrength::setValue(ActorComponent* component, float value, float mix)
{
	// Hard assumption this is a n IK target, this works because we don't allow deriving the IK Target.
	// May want to change this to static_cast for sanity if we ever allow the above.
	ActorIKTarget* ikTarget = reinterpret_cast<ActorIKTarget*>(component);
	ikTarget->strength(ikTarget->strength() * (1.0f - mix) + value * mix);
}