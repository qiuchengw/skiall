#include <math.h>
#include <iostream>
#include "KeyFrameSequence.hpp"
#include "../../ActorImage.hpp"

using namespace nima;

bool KeyFrameSequence::read(BlockReader* reader, ActorComponent* component)
{
    if(KeyFrameNumeric::read(reader, component))
    {
        return true;
    }

    return false;
}

void KeyFrameSequence::setValue(ActorComponent* component, float value, float mix)
{
    ActorImage* node = static_cast<ActorImage*>(component);
    int v = floor(value);
    int c = node->sequenceFramesCount();
    int frameIdx = v % c;
    if(frameIdx < 0)
    {
        frameIdx += node->sequenceFramesCount();
    }
    node->sequenceFrame(frameIdx);
}