#include "PropertyAnimation.hpp"
#include "../BlockReader.hpp"
#include "KeyFrames/KeyFrame.hpp"
#include "KeyFrames/KeyFramePosX.hpp"
#include "KeyFrames/KeyFramePosY.hpp"
#include "KeyFrames/KeyFrameScaleX.hpp"
#include "KeyFrames/KeyFrameScaleY.hpp"
#include "KeyFrames/KeyFrameRotation.hpp"
#include "KeyFrames/KeyFrameOpacity.hpp"
#include "KeyFrames/KeyFrameDrawOrder.hpp"
#include "KeyFrames/KeyFrameLength.hpp"
#include "KeyFrames/KeyFrameVertexDeform.hpp"
#include "KeyFrames/KeyFrameIKStrength.hpp"
#include "KeyFrames/KeyFrameTrigger.hpp"
#include "KeyFrames/KeyFrameCustomProperty.hpp"
#include "KeyFrames/KeyFrameIsCollisionEnabled.hpp"
#include "KeyFrames/KeyFrameActiveChild.hpp"
#include "KeyFrames/KeyFrameSequence.hpp"
#include <cassert> 

using namespace nima;

PropertyAnimation::PropertyAnimation() :
	m_Type(PropertyType::Unknown),
	m_KeyFrames(nullptr),
	m_KeyFramesCount(0)
{

}

PropertyAnimation::~PropertyAnimation()
{
	for(int i = 0; i < m_KeyFramesCount; i++)
	{
		delete m_KeyFrames[i];
	}
	delete [] m_KeyFrames;
}

int PropertyAnimation::keyFramesCount() const
{
	return m_KeyFramesCount;
}

PropertyType PropertyAnimation::type() const
{
	return m_Type;
}

void PropertyAnimation::read(BlockReader* reader, ActorComponent* component)
{
	BlockReader* block = reader->readNextBlock();
	if(block == nullptr)
	{
		return;
	}
	m_Type = block->blockType<PropertyType>();
	if(m_Type >= PropertyType::Max)
	{
		block->close();
		return;
	}
	m_KeyFramesCount = (int)block->readUnsignedShort();
	m_KeyFrames = new KeyFrame*[m_KeyFramesCount];
	KeyFrame* lastKeyFrame = nullptr;
	int currentIndex = 0;
	for(int i = 0; i < m_KeyFramesCount; i++)
	{
		KeyFrame* frame = nullptr;
		switch(m_Type)
		{
			case PropertyType::PosX:
				frame = new KeyFramePosX();
				break;
			case PropertyType::PosY:
				frame = new KeyFramePosY();
				break;
			case PropertyType::ScaleX:
				frame = new KeyFrameScaleX();
				break;
			case PropertyType::ScaleY:
				frame = new KeyFrameScaleY();
				break;
			case PropertyType::Rotation:
				frame = new KeyFrameRotation();
				break;
			case PropertyType::Opacity:
				frame = new KeyFrameOpacity();
				break;
			case PropertyType::DrawOrder:
				frame = new KeyFrameDrawOrder();
				break;
			case PropertyType::Length:
				frame = new KeyFrameLength();
				break;
			case PropertyType::VertexDeform:
				frame = new KeyFrameVertexDeform();
				break;
			case PropertyType::IKStrength:
				frame = new KeyFrameIKStrength();
				break;
			case PropertyType::Trigger:
				frame = new KeyFrameTrigger();
				break;
			case PropertyType::IntProperty:
				frame = new KeyFrameIntProperty();
				break;
			case PropertyType::FloatProperty:
				frame = new KeyFrameFloatProperty();
				break;
			case PropertyType::StringProperty:
				frame = new KeyFrameStringProperty();
				break;
			case PropertyType::BooleanProperty:
				frame = new KeyFrameBooleanProperty();
				break;
			case PropertyType::IsCollisionEnabled:
				frame = new KeyFrameIsCollisionEnabled();
				break;
			case PropertyType::ActiveChildIndex:
				frame = new KeyFrameActiveChild();
				break;
			case PropertyType::Sequence:
				frame = new KeyFrameSequence();
				break;
			default:
				// This will only happen if the code isn't handling a property type it should handle.
				// Check the PropertyType enum and make sure Max is in the right place (and that you're not missing a case).
				assert(false);
				break;
		}

		if(frame->read(block, component))
		{
			m_KeyFrames[currentIndex++] = frame;
			if(lastKeyFrame != nullptr)
			{
				lastKeyFrame->setNext(frame);
			}
			lastKeyFrame = frame;
		}
		else
		{
			// If the frame fails to load, we just remove it from the list, not we change our list length (no we don't realloc the list length).
			delete frame;
			m_KeyFramesCount--;
		}
	}
	block->close();
}

void PropertyAnimation::apply(float time, ActorComponent* component, float mix)
{
	if(m_KeyFramesCount == 0)
	{
		return;
	}

	int idx = 0;
	// Binary find the keyframe index.
	{
		int mid = 0;
		float element = 0.0f;
		int start = 0;
		int end = m_KeyFramesCount-1;

		while (start <= end) 
		{
	    	mid = ((start + end) >> 1);
			element = m_KeyFrames[mid]->time();
			if (element < time) 
			{
				start = mid + 1;
			} 
			else if (element > time) 
			{
				end = mid - 1;
			} 
			else 
			{
				start = mid;
				break;
			}
		}

		idx = start;
	}

	if(idx == 0)
	{
		m_KeyFrames[0]->apply(component, mix);
	}
	else
	{
		if(idx < m_KeyFramesCount)
		{
			KeyFrame* fromFrame = m_KeyFrames[idx-1];
			KeyFrame* toFrame = m_KeyFrames[idx];
			if(time == toFrame->time())
			{
				toFrame->apply(component, mix);
			}
			else
			{
				fromFrame->applyInterpolation(component, time, toFrame, mix);
			}
		}
		else
		{
			m_KeyFrames[idx-1]->apply(component, mix);
		}
	}
}