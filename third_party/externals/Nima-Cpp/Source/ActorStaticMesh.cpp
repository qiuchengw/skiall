#include "ActorStaticMesh.hpp"
#include "ActorBone.hpp"
#include "BlockReader.hpp"
#include "Actor.hpp"
#include <cstring>

using namespace nima;

ActorStaticMesh::ActorStaticMesh() :
	ActorRenderNode(ComponentType::ActorStaticMesh),

	m_IsInstance(false),
	m_BlendMode(BlendMode::Normal),
	m_NumSurfaces(0),
	m_Surfaces(nullptr)
{

}

ActorStaticMesh::Surface::Surface() : 
	textureIndex(0),
	vertexCount(0),
	triangleCount(0),
	vertices(nullptr),
	triangles(nullptr)
{

}

ActorStaticMesh::Surface::~Surface()
{
	disposeGeometry();
}

void ActorStaticMesh::Surface::disposeGeometry()
{
	delete [] vertices;
	delete [] triangles;

	vertices = nullptr;
	triangles = nullptr;
}

ActorStaticMesh::~ActorStaticMesh()
{
	if(m_IsInstance)
	{
		return;
	}

	for(int i = 0; i < m_NumSurfaces; i++)
	{
		delete m_Surfaces[i]; 
	}
	delete [] m_Surfaces;
}

ActorComponent* ActorStaticMesh::makeInstance(Actor* resetActor)
{
	ActorStaticMesh* instanceNode = new ActorStaticMesh();
	instanceNode->copy(this, resetActor);
	return instanceNode;
}


void ActorStaticMesh::disposeGeometry()
{
	if(m_IsInstance)
	{
		return;
	}

	for(int i = 0; i < m_NumSurfaces; i++)
	{
		m_Surfaces[i]->disposeGeometry();
	}
//	delete [] m_Vertices;
//	m_Vertices = nullptr;
//	delete [] m_Triangles;
//	m_Triangles = nullptr;
}


void ActorStaticMesh::copy(ActorStaticMesh* node, Actor* resetActor)
{
	Base::copy(node, resetActor);

	m_IsInstance = true;
	m_BlendMode = node->m_BlendMode;
	m_NumSurfaces = node->m_NumSurfaces;
	m_Surfaces = node->m_Surfaces;
}

ActorStaticMesh::Surface* ActorStaticMesh::makeSurface()
{
	return new Surface();
}

ActorStaticMesh* ActorStaticMesh::read(Actor* actor, BlockReader* reader, ActorStaticMesh* node)
{
	if (node == nullptr)
	{
		node = new ActorStaticMesh();
	}

	Base::read(actor, reader, node);

	bool isVisible = reader->readByte() != 0;
	if (isVisible)
	{
		node->m_BlendMode = (BlendMode)reader->readByte();
		node->m_DrawOrder = (int)reader->readUnsignedShort();

		int surfaceCount = (int)reader->readUnsignedShort();
		node->m_NumSurfaces = surfaceCount;
		node->m_Surfaces = new Surface*[surfaceCount];

		for(int i = 0; i < surfaceCount; i++)
		{
			auto surface = node->makeSurface();
			node->m_Surfaces[i] = surface;
			surface->textureIndex = (int)reader->readByte();
			
			surface->vertexCount = reader->readUnsignedInt();
			const int vertexStride = 4;
			unsigned int vertexLength = surface->vertexCount * vertexStride;
			surface->vertices = new float[vertexLength];
			reader->readFloatArray(surface->vertices, vertexLength);

			surface->triangleCount = reader->readUnsignedInt();
			unsigned int triLength = surface->triangleCount * 3;
			surface->triangles = new unsigned short[triLength];
			reader->readUnsignedShortArray(surface->triangles, triLength);
		}
	}
	return node;
}

void ActorStaticMesh::resolveComponentIndices(ActorComponent** components)
{
	Base::resolveComponentIndices(components);
}

BlendMode ActorStaticMesh::blendMode() const
{
	return m_BlendMode;
}

int ActorStaticMesh::numSurfaces() const
{
	return m_NumSurfaces;
}

ActorStaticMesh::Surface* ActorStaticMesh::surface(int index)
{
	return m_Surfaces[index];
}