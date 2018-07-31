#include "Actor.hpp"
#include "ActorBone.hpp"
#include "ActorRootBone.hpp"
#include "ActorIKTarget.hpp"
#include "ActorEvent.hpp"
#include "ActorNodeSolo.hpp"
#include "CustomProperty.hpp"
#include "ActorCollider.hpp"
#include "BinaryReader.hpp"
#include "BlockReader.hpp"
#include "Exceptions/OverflowException.hpp"
#include "Exceptions/UnsupportedVersionException.hpp"
#include "Exceptions/MissingFileException.hpp"
#include <stdio.h>
#include <algorithm>
#include <assert.h>

using namespace nima;

Actor::Actor() :
	m_Flags(IsDrawOrderDirty | IsVertexDeformDirty),
	m_ComponentCount(0),
	m_NodeCount(0),
	m_Components(nullptr),
	m_Nodes(nullptr),
	m_Root(nullptr),
	m_EventCallbackUserData(nullptr),
	m_EventCallback(nullptr),
	m_MaxTextureIndex(-1),
	m_ImageNodeCount(0),
	m_RenderNodeCount(0),
	m_SolverNodeCount(0),
	m_AnimationsCount(0),
	m_NestedActorAssetCount(0),
	m_NestedActorNodeCount(0),
	m_ImageNodes(nullptr),
	m_RenderNodes(nullptr),
	m_Solvers(nullptr),
	m_Animations(nullptr),
	m_NestedActorAssets(nullptr),
	m_NestedActorNodes(nullptr)

{

}

Actor::~Actor()
{
	dispose();
}

void Actor::dispose()
{
	for (unsigned int i = 0; i < m_ComponentCount; i++)
	{
		delete m_Components[i];
	}
	delete [] m_Components;
	delete [] m_Nodes;
	delete [] m_ImageNodes;
	delete [] m_NestedActorNodes;
	delete [] m_Solvers;
	delete [] m_RenderNodes;
	if ((m_Flags & IsInstance) == 0x0)
	{
		delete [] m_Animations;

		for (int i = 0; i < m_NestedActorAssetCount; i++)
		{
			delete m_NestedActorAssets[i];
		}
		delete [] m_NestedActorAssets;
	}


	m_ComponentCount = 0;
	m_NodeCount = 0;
	m_MaxTextureIndex = -1;
	m_ImageNodeCount = 0;
	m_RenderNodeCount = 0;
	m_SolverNodeCount = 0;
	m_AnimationsCount = 0;
	m_NestedActorAssetCount = 0;
	m_Components = nullptr;
	m_Nodes = nullptr;
	m_ImageNodes = nullptr;
	m_RenderNodes = nullptr;
	m_Solvers = nullptr;
	m_Animations = nullptr;
	m_NestedActorAssets = nullptr;
	m_Root = nullptr;
	m_NestedActorNodes = nullptr;
}

ActorNode* Actor::root() const
{
	return m_Root;
}

ActorComponent* Actor::component(unsigned int index) const
{
	return m_Components[index];
}

ActorComponent* Actor::component(unsigned short index) const
{
	return m_Components[index];
}

ActorComponent* Actor::component(const std::string& name) const
{
	for (unsigned int i = 0; i < m_ComponentCount; i++)
	{
		ActorComponent* a = m_Components[i];
		if (a->name() == name)
		{
			return a;
		}
	}
	return nullptr;
}

NestedActorAsset* Actor::nestedActorAsset(unsigned int index) const
{
	if (index < m_NestedActorAssetCount)
	{
		return m_NestedActorAssets[index];
	}
	return nullptr;
}

void Actor::eventCallback(ActorAnimationEvent::Callback callback, void* userdata)
{
	m_EventCallbackUserData = userdata;
	m_EventCallback = callback;
}

ActorAnimation* Actor::animation(const std::string& name) const
{
	for (int i = 0; i < m_AnimationsCount; i++)
	{
		ActorAnimation& a = m_Animations[i];
		if (a.name() == name)
		{
			return &a;
		}
	}
	return nullptr;
}

ActorAnimationInstance* Actor::animationInstance(const std::string& name)
{
	ActorAnimation* a = animation(name);
	if (a == nullptr)
	{
		return nullptr;
	}
	return new ActorAnimationInstance(this, a);
}

void Actor::load(unsigned char* bytes, unsigned int length)
{
	dispose();

	BlockReader reader(bytes, length);

	unsigned char N = reader.readByte();
	unsigned char I = reader.readByte();
	unsigned char M = reader.readByte();
	unsigned char A = reader.readByte();
	unsigned int version = reader.readUnsignedInt();

	// Make sure it's a nima file.
	if (N != 78 || I != 73 || M != 77 || A != 65)
	{
		throw UnsupportedVersionException("Unsupported file version", 0, 12);
	}
	// And of supported version...
	if (version < 13)
	{
		throw UnsupportedVersionException("Unsupported file version", version, 12);
	}

	m_Root = new ActorNode();
	BlockReader* block = nullptr;
	while ((block = reader.readNextBlock()) != nullptr)
	{
		switch (block->blockType<BlockType>())
		{
			case BlockType::Components:
				readComponentsBlock(block);
				break;
			case BlockType::Animations:
				readAnimationsBlock(block);
				break;
			case BlockType::NestedActorAssets:
				readNestedActorAssetsBlock(block);
				break;
			default:
				break;
		}
		block->close();
	}
}

void Actor::setFilename(const std::string& filename)
{
	size_t index = filename.rfind('.');
	if (index == std::string::npos)
	{
		m_BaseFilename = filename;
	}
	else
	{
		m_BaseFilename = std::string(filename, 0, index);
	}
}

void Actor::load(const std::string& filename)
{
	setFilename(filename);

	FILE* fp = fopen(filename.c_str(), "rb");
	if (fp == nullptr)
	{
		throw MissingFileException("nima file is missing", filename);
	}
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* bytes = new unsigned char[length];
	fread(bytes, length, 1, fp);
	fclose(fp);

	try
	{
		load(bytes, (unsigned int)length);
		delete [] bytes;
	}
	catch (OverflowException ex)
	{
		delete [] bytes;
		throw ex;
	}
}

void Actor::readNestedActorAssetsBlock(BlockReader* block)
{
	m_NestedActorAssetCount = (int)block->readUnsignedShort();
	m_NestedActorAssets = new NestedActorAsset*[m_NestedActorAssetCount];

	BlockReader* nestedActorAssetBlock = nullptr;
	int nestedActorIndex = 0;

	while ((nestedActorAssetBlock = block->readNextBlock()) != nullptr)
	{
		switch (nestedActorAssetBlock->blockType<BlockType>())
		{
			case BlockType::NestedActorAsset:
				// Sanity check.
				if (nestedActorIndex < m_NestedActorAssetCount)
				{
					NestedActorAsset* asset = makeNestedActorAsset();
					asset->read(nestedActorAssetBlock);
					m_NestedActorAssets[nestedActorIndex++] = asset;
				}
				break;
			default:
				break;
		}

		nestedActorAssetBlock->close();
	};
}

void Actor::readAnimationsBlock(BlockReader* block)
{
	m_AnimationsCount = (int)block->readUnsignedShort();
	m_Animations = new ActorAnimation[m_AnimationsCount];

	BlockReader* animationBlock = nullptr;
	int animationIndex = 0;

	while ((animationBlock = block->readNextBlock()) != nullptr)
	{
		switch (animationBlock->blockType<BlockType>())
		{
			case BlockType::Animation:
				// Sanity check.
				if (animationIndex < m_AnimationsCount)
				{
					m_Animations[animationIndex++].read(animationBlock, m_Components);
				}
				break;
			default:
				break;
		}

		animationBlock->close();
	};
}

ActorImage* Actor::makeImageNode()
{
	return new ActorImage();
}

ActorStaticMesh* Actor::makeStaticMeshNode()
{
	return new ActorStaticMesh();
}

NestedActorNode* Actor::makeNestedActorNode()
{
	return new NestedActorNode();
}

NestedActorAsset* Actor::makeNestedActorAsset()
{
	return new NestedActorAsset();
}

void Actor::readComponentsBlock(BlockReader* block)
{
	m_ComponentCount = (unsigned int)(block->readUnsignedShort() + 1);
	m_Components = new ActorComponent*[m_ComponentCount];
	m_Components[0] = m_Root;

	BlockReader* componentBlock = nullptr;
	int componentIndex = 1;
	m_NodeCount = 1;
	while ((componentBlock = block->readNextBlock()) != nullptr)
	{
		ActorComponent* component = nullptr;
		switch (componentBlock->blockType<BlockType>())
		{
			case BlockType::ActorNode:
				component = ActorNode::read(this, componentBlock);
				break;
			case BlockType::ActorBone:
				component = ActorBone::read(this, componentBlock);
				break;
			case BlockType::ActorRootBone:
				component = ActorRootBone::read(this, componentBlock);
				break;
			case BlockType::ActorImage:
			{
				m_RenderNodeCount++;
				m_ImageNodeCount++;
				component = ActorImage::read(this, componentBlock, makeImageNode());
				ActorImage* imageNode = static_cast<ActorImage*>(component);
				if (imageNode->textureIndex() > m_MaxTextureIndex)
				{
					m_MaxTextureIndex = imageNode->textureIndex();
				}
				break;
			}
			case BlockType::ActorImageSequence:
			{
				m_RenderNodeCount++;
				m_ImageNodeCount++;
				component = ActorImage::readImageSequence(this, componentBlock, makeImageNode());
				ActorImage* imageNode = static_cast<ActorImage*>(component);
				SequenceFrame* frames = imageNode->sequenceFrames();
				m_MaxTextureIndex = frames[imageNode->sequenceFramesCount() - 1].m_AtlasIndex; // Last SequenceFrame has the biggest texture index
				break;
			}
			case BlockType::ActorStaticMesh:
			{
				m_RenderNodeCount++;
				component = ActorStaticMesh::read(this, componentBlock, makeStaticMeshNode());

				ActorStaticMesh* staticMeshNode = static_cast<ActorStaticMesh*>(component);
				for (int i = 0; i < staticMeshNode->numSurfaces(); i++)
				{
					ActorStaticMesh::Surface* surface = staticMeshNode->surface(i);
					if (surface->textureIndex > m_MaxTextureIndex)
					{
						m_MaxTextureIndex = surface->textureIndex;
					}
				}
				break;
			}
			case BlockType::NestedActorNode:
			{
				m_RenderNodeCount++;
				m_NestedActorNodeCount++;
				component = NestedActorNode::read(this, componentBlock, makeNestedActorNode());
				break;
			}
			case BlockType::ActorIKTarget:
				m_SolverNodeCount++;
				component = ActorIKTarget::read(this, componentBlock);
				break;
			case BlockType::ActorEvent:
				component = ActorEvent::read(this, componentBlock);
				break;
			case BlockType::CustomIntProperty:
				component = CustomIntProperty::read(this, componentBlock);
				break;
			case BlockType::CustomFloatProperty:
				component = CustomFloatProperty::read(this, componentBlock);
				break;
			case BlockType::CustomStringProperty:
				component = CustomStringProperty::read(this, componentBlock);
				break;
			case BlockType::CustomBooleanProperty:
				component = CustomBooleanProperty::read(this, componentBlock);
				break;
			case BlockType::ColliderRectangle:
				component = ActorColliderRectangle::read(this, componentBlock);
				break;
			case BlockType::ColliderTriangle:
				component = ActorColliderTriangle::read(this, componentBlock);
				break;
			case BlockType::ColliderCircle:
				component = ActorColliderCircle::read(this, componentBlock);
				break;
			case BlockType::ColliderPolygon:
				component = ActorColliderPolygon::read(this, componentBlock);
				break;
			case BlockType::ColliderLine:
				component = ActorColliderLine::read(this, componentBlock);
				break;
			case BlockType::ActorNodeSolo:
				component = ActorNodeSolo::read(this, componentBlock);
				break;
			default:
				// Not handled/expected block.
				break;
		}
		if (component != nullptr && component->isNode())
		{
			m_NodeCount++;
		}
		m_Components[componentIndex] = component;
		componentIndex++;

		componentBlock->close();
	}

	m_Nodes = new ActorNode*[m_NodeCount];
	m_ImageNodes = new ActorImage*[m_ImageNodeCount];
	m_RenderNodes = new ActorRenderNode*[m_RenderNodeCount];
	m_Solvers = new Solver*[m_SolverNodeCount];
	m_NestedActorNodes = new NestedActorNode*[m_NestedActorNodeCount];
	m_Nodes[0] = m_Root;

	// Resolve nodes.
	int imdIdx = 0;
	int rndIdx = 0;
	int slvIdx = 0;
	int ndeIdx = 1;
	int nanIdx = 0;

	for (unsigned int i = 1; i < m_ComponentCount; i++)
	{
		ActorComponent* component = m_Components[i];
		if (component != nullptr)
		{
			component->resolveComponentIndices(m_Components);

			switch (component->type())
			{
				case ComponentType::NestedActorNode:
					m_NestedActorNodes[nanIdx++] = static_cast<NestedActorNode*>(component);
					m_RenderNodes[rndIdx++] = static_cast<NestedActorNode*>(component);
					break;
				case ComponentType::ActorStaticMesh:
					m_RenderNodes[rndIdx++] = static_cast<ActorStaticMesh*>(component);
					break;
				case ComponentType::ActorImage:
					m_RenderNodes[rndIdx++] = static_cast<ActorImage*>(component);
					m_ImageNodes[imdIdx++] = static_cast<ActorImage*>(component);
					break;
				case ComponentType::ActorIKTarget:
					m_Solvers[slvIdx++] = static_cast<ActorIKTarget*>(component);
					break;
				default:
					break;
			}

			if (component->isNode())
			{
				m_Nodes[ndeIdx++] = static_cast<ActorNode*>(component);
			}
		}
	}
}

static bool DrawOrderComparer(ActorRenderNode* a, ActorRenderNode* b)
{
	return a->drawOrder() < b->drawOrder();
}

static bool SolverComparer(Solver* a, Solver* b)
{
	return a->order() < b->order();
}

void Actor::copy(const Actor& actor)
{
	m_Flags = actor.m_Flags;
	m_Flags |= IsInstance;
	m_Animations = actor.m_Animations;
	m_AnimationsCount = actor.m_AnimationsCount;
	m_MaxTextureIndex = actor.m_MaxTextureIndex;
	m_ImageNodeCount = actor.m_ImageNodeCount;
	m_RenderNodeCount = actor.m_RenderNodeCount;
	m_SolverNodeCount = actor.m_SolverNodeCount;
	m_ComponentCount = actor.m_ComponentCount;
	m_NodeCount = actor.m_NodeCount;
	m_NestedActorAssetCount = actor.m_NestedActorAssetCount;
	m_NestedActorNodeCount = actor.m_NestedActorNodeCount;
	m_BaseFilename = actor.m_BaseFilename;
	m_NestedActorAssets = actor.m_NestedActorAssets;

	if (m_ComponentCount != 0)
	{
		m_Components = new ActorComponent*[m_ComponentCount];
	}
	if (m_NodeCount != 0)
	{
		m_Nodes = new ActorNode*[m_NodeCount];
	}
	if (m_RenderNodeCount != 0)
	{
		m_RenderNodes = new ActorRenderNode*[m_RenderNodeCount];
	}
	if (m_ImageNodeCount != 0)
	{
		m_ImageNodes = new ActorImage*[m_ImageNodeCount];
	}
	if (m_SolverNodeCount != 0)
	{
		m_Solvers = new Solver*[m_SolverNodeCount];
	}
	if (m_NestedActorNodeCount != 0)
	{
		m_NestedActorNodes = new NestedActorNode*[m_NestedActorNodeCount];
	}

	if (m_ComponentCount > 0)
	{
		int idx = 0;
		int rndIdx = 0;
		int imgIdx = 0;
		int slvIdx = 0;
		int ndeIdx = 0;
		int nanIdx = 0;

		for (int i = 0; i < m_ComponentCount; i++)
		{
			ActorComponent* component = actor.m_Components[i];
			if (component == nullptr)
			{
				m_Components[idx++] = nullptr;
				continue;
			}
			ActorComponent* instanceComponent = component->makeInstance(this);
			m_Components[idx++] = instanceComponent;
			switch (instanceComponent->type())
			{
				case ComponentType::NestedActorNode:
					m_NestedActorNodes[nanIdx++] = static_cast<NestedActorNode*>(instanceComponent);
					m_RenderNodes[rndIdx++] = static_cast<NestedActorNode*>(instanceComponent);
					break;
				case ComponentType::ActorStaticMesh:
					m_RenderNodes[rndIdx++] = static_cast<ActorStaticMesh*>(instanceComponent);
					break;
				case ComponentType::ActorImage:
					m_RenderNodes[rndIdx++] = static_cast<ActorImage*>(instanceComponent);
					m_ImageNodes[imgIdx++] = static_cast<ActorImage*>(instanceComponent);
					break;
				case ComponentType::ActorIKTarget:
					m_Solvers[slvIdx++] = static_cast<ActorIKTarget*>(instanceComponent);
					break;
				default:
					break;
			}

			if (instanceComponent->isNode())
			{
				m_Nodes[ndeIdx++] = static_cast<ActorNode*>(instanceComponent);
			}
		}

		// Resolve indices.
		m_Root = m_Nodes[0];
		for (int i = 1; i < m_ComponentCount; i++)
		{
			ActorComponent* component = m_Components[i];
			if (component == nullptr)
			{
				continue;
			}
			component->resolveComponentIndices(m_Components);
		}

		if (m_Solvers != nullptr)
		{
			std::sort(m_Solvers, m_Solvers + m_SolverNodeCount, SolverComparer);
		}
	}
}

const int Actor::nestedActorCount() const
{
	return m_NestedActorAssetCount;
}

const int Actor::textureCount() const
{
	return m_MaxTextureIndex + 1;
}

const std::string& Actor::baseFilename() const
{
	return m_BaseFilename;
}

void Actor::markDrawOrderDirty()
{
	m_Flags |= IsDrawOrderDirty;
}

void Actor::advance(float elapsedSeconds)
{
	bool runSolvers = false;
	for (int i = 0; i < m_SolverNodeCount; i++)
	{
		Solver* solver = m_Solvers[i];
		if (solver != nullptr && solver->needsSolve())
		{
			runSolvers = true;
			break;
		}
	}

	for (unsigned int i = 0; i < m_NodeCount; i++)
	{
		ActorNode* node = m_Nodes[i];
		if (node != nullptr)
		{
			node->updateTransforms();
		}
	}

	if (runSolvers)
	{

		for (unsigned int i = 0; i < m_SolverNodeCount; i++)
		{
			Solver* solver = m_Solvers[i];
			if (solver != nullptr)
			{
				solver->solveStart();
			}
		}

		for (unsigned int i = 0; i < m_SolverNodeCount; i++)
		{
			Solver* solver = m_Solvers[i];
			if (solver != nullptr)
			{
				solver->solve();
			}
		}

		for (unsigned int i = 0; i < m_SolverNodeCount; i++)
		{
			Solver* solver = m_Solvers[i];
			if (solver != nullptr)
			{
				solver->suppressMarkDirty(true);
			}
		}

		for (unsigned int i = 0; i < m_NodeCount; i++)
		{
			ActorNode* node = m_Nodes[i];
			if (node != nullptr)
			{
				node->updateTransforms();
			}
		}

		for (unsigned int i = 0; i < m_SolverNodeCount; i++)
		{
			Solver* solver = m_Solvers[i];
			if (solver != nullptr)
			{
				solver->suppressMarkDirty(false);
			}
		}
	}

	if ((m_Flags & IsDrawOrderDirty) != 0)
	{
		m_Flags &= ~IsDrawOrderDirty;

		if (m_RenderNodes != nullptr)
		{
			std::sort(m_RenderNodes, m_RenderNodes + m_RenderNodeCount, DrawOrderComparer);
		}
	}
	if ((m_Flags & IsVertexDeformDirty) != 0)
	{
		m_Flags &= ~IsVertexDeformDirty;
		for (unsigned int i = 0; i < m_ImageNodeCount; i++)
		{
			ActorImage* imageNode = m_ImageNodes[i];
			if (imageNode != nullptr && imageNode->isVertexDeformDirty())
			{
				imageNode->isVertexDeformDirty(false);
				updateVertexDeform(imageNode);
			}
		}
	}

	// Advance nested.
	for (unsigned int i = 0; i < m_NestedActorNodeCount; i++)
	{
		m_NestedActorNodes[i]->advance(elapsedSeconds);
	}
}

Actor* Actor::makeInstance() const
{
	assert(false);
	return nullptr;
}

unsigned int Actor::componentCount() const
{
	return m_ComponentCount;
}