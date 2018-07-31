#include "KeyFrameVertexDeform.hpp"
#include "../../ActorImage.hpp"
#include "../../BlockReader.hpp"

using namespace nima;

KeyFrameVertexDeform::KeyFrameVertexDeform() :
	m_Vertices(nullptr),
	m_VerticesCount(0)
{

}

KeyFrameVertexDeform::~KeyFrameVertexDeform()
{
	delete [] m_Vertices;
}

bool KeyFrameVertexDeform::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}

	ActorImage* imageNode = reinterpret_cast<ActorImage*>(component);
	if(imageNode == nullptr)
	{
		return false;
	}
	m_VerticesCount = imageNode->vertexCount() * 2;
	m_Vertices = new float[m_VerticesCount];
	reader->readFloatArray(m_Vertices, m_VerticesCount);
	
	imageNode->doesAnimationVertexDeform(true);
	return true;
}

void KeyFrameVertexDeform::apply(ActorComponent* component, float mix)
{
	ActorImage* imageNode = reinterpret_cast<ActorImage*>(component);
	if(imageNode == nullptr)
	{
		return;
	}
	int l = m_VerticesCount;
	float* wr = imageNode->animationDeformedVertices();
	if(mix == 1.0f)
	{
		for(int i = 0; i < l; i++)
		{
			wr[i] = m_Vertices[i];
		}
	}
	else
	{
		float mixi = 1.0f - mix;
		for(int i = 0; i < l; i++)
		{
			wr[i] = wr[i] * mixi + m_Vertices[i] * mix;
		}
	}

	imageNode->isVertexDeformDirty(true);
}

void KeyFrameVertexDeform::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	ActorImage* imageNode = reinterpret_cast<ActorImage*>(component);
	if(imageNode == nullptr)
	{
		return;
	}
	float* wr = imageNode->animationDeformedVertices();
	float* to = reinterpret_cast<KeyFrameVertexDeform*>(toFrame)->m_Vertices;
	int l = m_VerticesCount;

	float f = (time - m_Time)/(toFrame->time()-m_Time);
	float fi = 1.0f - f;
	if(mix == 1.0f)
	{
		for(int i = 0; i < l; i++)
		{
			wr[i] = m_Vertices[i] * fi + to[i] * f;
		}
	}
	else
	{
		float mixi = 1.0f - mix;
		for(int i = 0; i < l; i++)
		{
			float v = m_Vertices[i] * fi + to[i] * f;

			wr[i] = wr[i] * mixi + v * mix;
		}
	}

	imageNode->isVertexDeformDirty(true);
}