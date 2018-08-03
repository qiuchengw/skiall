#include "ActorImage.hpp"
#include "ActorBone.hpp"
#include "BlockReader.hpp"
#include "Actor.hpp"
#include <cstring>

using namespace nima;

ActorImage::ActorImage() :
	ActorRenderNode(ComponentType::ActorImage),

	m_IsInstance(false),
	m_BlendMode(BlendMode::Normal),
	m_TextureIndex(-1),
	m_Vertices(nullptr),
	m_Triangles(nullptr),
	m_VertexCount(0),
	m_TriangleCount(0),
	m_AnimationDeformedVertices(nullptr),
	m_IsVertexDeformDirty(false),
	m_BoneMatrices(nullptr),
	m_NumConnectedBones(0),
	m_BoneConnections(nullptr),
	m_SequenceFrames(nullptr),
	m_SequenceUVs(nullptr),
	m_SequenceFrame(0),
	m_SequenceFramesCount(0)
{

}

ActorImage::BoneConnection::BoneConnection() : boneIndex(0), node(nullptr)
{

}

ActorImage::~ActorImage()
{
	if(!m_IsInstance)
	{
		delete [] m_Vertices;
		delete [] m_Triangles;
		if(m_SequenceFramesCount > 0)
		{
			delete [] m_SequenceUVs;
			delete [] m_SequenceFrames;
		}
	}
	delete [] m_AnimationDeformedVertices;
	delete [] m_BoneMatrices;
	delete [] m_BoneConnections;
}

ActorComponent* ActorImage::makeInstance(Actor* resetActor)
{
	ActorImage* instanceNode = new ActorImage();
	instanceNode->copy(this, resetActor);
	return instanceNode;
}

bool ActorImage::doesAnimationVertexDeform() const
{
	return m_AnimationDeformedVertices != nullptr;
}

void ActorImage::doesAnimationVertexDeform(bool doesIt)
{
	if (doesIt)
	{
		delete [] m_AnimationDeformedVertices;
		m_AnimationDeformedVertices = new float [m_VertexCount * 2];

		// Copy the deform verts from the rig verts.
		int writeIdx = 0;
		int readIdx = 0;
		int readStride = vertexStride();
		for(int i = 0; i < m_VertexCount; i++)
		{
			m_AnimationDeformedVertices[writeIdx++] = m_Vertices[readIdx];
			m_AnimationDeformedVertices[writeIdx++] = m_Vertices[readIdx+1];
			readIdx += readStride;
		}
	}
	else
	{
		delete [] m_AnimationDeformedVertices;
		m_AnimationDeformedVertices = nullptr;
	}
}

float* ActorImage::animationDeformedVertices()
{
	return m_AnimationDeformedVertices;
}

bool ActorImage::isVertexDeformDirty() const
{
	return m_IsVertexDeformDirty;
}

void ActorImage::isVertexDeformDirty(bool isIt)
{
	m_IsVertexDeformDirty = isIt;
}

void ActorImage::disposeGeometry()
{
	if(m_IsInstance)
	{
		return;
	}
	delete [] m_Vertices;
	m_Vertices = nullptr;
	delete [] m_Triangles;
	m_Triangles = nullptr;
}

int ActorImage::boneInfluenceMatricesLength()
{
	return m_NumConnectedBones == 0 ? 0 : (m_NumConnectedBones + 1) * 6;
}
float* ActorImage::boneInfluenceMatrices()
{
	if (m_BoneMatrices == nullptr)
	{
		m_BoneMatrices = new float[boneInfluenceMatricesLength()];
		// First bone transform is always identity.
		m_BoneMatrices[0] = 1.0f;
		m_BoneMatrices[1] = 0.0f;
		m_BoneMatrices[2] = 0.0f;
		m_BoneMatrices[3] = 1.0f;
		m_BoneMatrices[4] = 0.0f;
		m_BoneMatrices[5] = 0.0f;
	}

	Mat2D mat;
	int bidx = 6;
	for (int i = 0; i < m_NumConnectedBones; i++)
	{
		BoneConnection& bc = m_BoneConnections[i];
		bc.node->updateTransforms();
		Mat2D::multiply(mat, bc.node->worldTransform(), bc.ibind);
		m_BoneMatrices[bidx++] = mat[0];
		m_BoneMatrices[bidx++] = mat[1];
		m_BoneMatrices[bidx++] = mat[2];
		m_BoneMatrices[bidx++] = mat[3];
		m_BoneMatrices[bidx++] = mat[4];
		m_BoneMatrices[bidx++] = mat[5];
	}

	return m_BoneMatrices;
}

void ActorImage::copy(ActorImage* node, Actor* resetActor)
{
	Base::copy(node, resetActor);

	m_IsInstance = true;
	m_BlendMode = node->m_BlendMode;
	m_TextureIndex = node->m_TextureIndex;
	m_VertexCount = node->m_VertexCount;
	m_TriangleCount = node->m_TriangleCount;
	m_Vertices = node->m_Vertices;
	m_Triangles = node->m_Triangles;
	m_SequenceFrames = node->m_SequenceFrames;
	m_SequenceFramesCount = node->m_SequenceFramesCount;
	if (node->m_AnimationDeformedVertices != nullptr)
	{
		int deformedVertexLength = m_VertexCount * 2;
		m_AnimationDeformedVertices = new float[deformedVertexLength];
		std::memmove(m_AnimationDeformedVertices, node->m_AnimationDeformedVertices, deformedVertexLength * sizeof(float));
	}

	if (node->m_BoneConnections != nullptr)
	{
		m_NumConnectedBones = node->m_NumConnectedBones;
		m_BoneConnections = new BoneConnection[node->m_NumConnectedBones];
		for (int i = 0; i < m_NumConnectedBones; i++)
		{
			BoneConnection& bcT = m_BoneConnections[i];
			BoneConnection& bcF = node->m_BoneConnections[i];

			bcT.boneIndex = bcF.boneIndex;
			Mat2D::copy(bcT.bind, bcF.bind);
			Mat2D::copy(bcT.ibind, bcF.ibind);
		}
	}
}

ActorImage* ActorImage::readImageSequence(Actor* actor, BlockReader* reader, ActorImage* node)
{
	ActorImage::read(actor, reader, node);

	if(node->textureIndex() >= 0)
	{
		int frameAssetCount = (int)reader->readUnsignedShort();
		node->m_SequenceFramesCount = frameAssetCount;
		node->m_SequenceUVs = new float[node->vertexCount()*2*frameAssetCount];

		node->m_SequenceFrames = new SequenceFrame[frameAssetCount];
		// Init first frame
		node->m_SequenceFrames[0] = {node->textureIndex(), 0};
		int readIdx = 2;
		int writeIdx = 0;

		for(int i = 0; i < node->m_VertexCount; i++)
		{
			node->m_SequenceUVs[writeIdx++] = node->m_Vertices[readIdx];
			node->m_SequenceUVs[writeIdx++] = node->m_Vertices[readIdx + 1];
			readIdx += node->vertexStride();
		}

		int uvStride = node->vertexCount() * 2;
		int offset = uvStride;
		for(int i = 1; i < frameAssetCount; i++)
		{
			SequenceFrame& sf = node->m_SequenceFrames[i];
			sf = {reader->readByte(), offset * 4};
			reader->readFloatArray(node->m_SequenceUVs+offset, uvStride);
			offset += uvStride;
		}
	}
	return node;
}

ActorImage* ActorImage::read(Actor* actor, BlockReader* reader, ActorImage* node)
{
	if (node == nullptr)
	{
		node = new ActorImage();
	}

	Base::read(actor, reader, node);

	bool isVisible = reader->readByte() != 0;
	if (isVisible)
	{
		node->m_BlendMode = (BlendMode)reader->readByte();
		node->m_DrawOrder = (int)reader->readUnsignedShort();
		node->m_TextureIndex = (int)reader->readByte();

		node->m_NumConnectedBones = (int)reader->readByte();
		if (node->m_NumConnectedBones != 0)
		{
			node->m_BoneConnections = new BoneConnection[node->m_NumConnectedBones];
			for (int i = 0; i < node->m_NumConnectedBones; i++)
			{
				BoneConnection& bc = node->m_BoneConnections[i];
				bc.boneIndex = reader->readUnsignedShort();
				reader->read(bc.bind);
				Mat2D::invert(bc.ibind, bc.bind);
			}

			Mat2D worldOverride;
			reader->read(worldOverride);
			node->overrideWorldTransform(worldOverride);
		}

		unsigned int numVertices = reader->readUnsignedInt();
		int vertexStride = node->m_NumConnectedBones > 0 ? 12 : 4;
		node->m_VertexCount = (int)numVertices;

		unsigned int vertexLength = numVertices * vertexStride;
		node->m_Vertices = new float[vertexLength];
		reader->readFloatArray(node->m_Vertices, vertexLength);

		unsigned int numTris = reader->readUnsignedInt();
		node->m_TriangleCount = (int)numTris;

		unsigned int triLength = numTris * 3;
		node->m_Triangles = new unsigned short[triLength];
		reader->readUnsignedShortArray(node->m_Triangles, triLength);
	}
	return node;
}

void ActorImage::resolveComponentIndices(ActorComponent** components)
{
	Base::resolveComponentIndices(components);
	if (m_BoneConnections != nullptr)
	{
		for (int i = 0; i < m_NumConnectedBones; i++)
		{
			BoneConnection& bc = m_BoneConnections[i];

			ActorBone* bone = dynamic_cast<ActorBone*>(components[bc.boneIndex]);
			if(bone != nullptr)
			{
				bc.node = bone;
				bone->isConnectedToImage(true);
			}
		}
	}
}

int ActorImage::textureIndex() const
{
	return m_TextureIndex;
}

int ActorImage::vertexCount() const
{
	return m_VertexCount;
}

int ActorImage::triangleCount() const
{
	return m_TriangleCount;
}

float* ActorImage::vertices() const
{
	return m_Vertices;
}

unsigned short* ActorImage::triangles() const
{
	return m_Triangles;
}

int ActorImage::connectedBoneCount() const
{
	return m_NumConnectedBones;
}

int ActorImage::vertexStride() const
{
	return m_NumConnectedBones > 0 ? 12 : 4;
}

BlendMode ActorImage::blendMode() const
{
	return m_BlendMode;
}

int ActorImage::sequenceFramesCount() const
{
	return m_SequenceFramesCount;
}

SequenceFrame* ActorImage::sequenceFrames() const
{
	 return m_SequenceFrames;
}

float* ActorImage::sequenceUVs() const
{
	return m_SequenceUVs;
}

void ActorImage::sequenceFrame(int value)
{
	m_SequenceFrame = value;
}

int ActorImage::sequenceFrame() const
{
	return m_SequenceFrame;
}