#include "ActorNode.hpp"
#include "BlockReader.hpp"
#include <algorithm>
#include <cassert>
using namespace nima;

ActorNode::ActorNode(Actor* actor) : ActorNode(actor, ComponentType::ActorNode)
{

			
}

ActorNode::ActorNode(ComponentType type) : ActorNode(nullptr, type)
{

			
}

ActorNode::ActorNode(Actor* actor, ComponentType type) : 
		ActorComponent(actor, type),
		m_Rotation(0.0f),
		m_Scale(1.0f, 1.0f),
		m_Opacity(1.0f),
		m_RenderOpacity(1.0f),
		m_IsDirty(true),
		m_IsWorldDirty(true),
		m_SuppressMarkDirty(false),
		m_OverrideWorldTransform(false),
		m_OverrideRotation(false),
		m_IsCollapsedVisibility(false),
		m_RenderCollapsed(false),
		m_OverrideRotationValue(0.0f)
{

}

ActorNode::ActorNode() : ActorNode(nullptr, ComponentType::ActorNode)
{

}

ActorNode::~ActorNode()
{

}

bool ActorNode::suppressMarkDirty() const
{
	return m_SuppressMarkDirty;
}

void ActorNode::suppressMarkDirty(bool suppress)
{
	m_SuppressMarkDirty = suppress;
}

bool ActorNode::isWorldDirty() const
{
	return m_IsWorldDirty;
}

bool ActorNode::isDirty() const
{
	return m_IsDirty;
}

const Mat2D& ActorNode::transform()
{
	if(m_IsDirty)
	{
		updateTransform();
	}
	return m_Transform;
}

const Mat2D& ActorNode::worldTransform()
{
	if(m_IsWorldDirty)
	{
		updateWorldTransform();
	}
	return m_WorldTransform;
}

void ActorNode::overrideWorldTransform(const Mat2D& transform)
{
	m_OverrideWorldTransform = true;
	Mat2D::copy(m_WorldTransform, transform);
	markWorldDirty();
}

void ActorNode::clearWorldTransformOverride()
{
	if(!m_OverrideWorldTransform)
	{
		return;
	}
	m_OverrideWorldTransform = false;
	markWorldDirty();
}

float ActorNode::x() const
{
	return m_Translation[0];
}

void ActorNode::x(float v)
{
	if(m_Translation[0] == v)
	{
		return;
	}
	m_Translation[0] = v;
	markDirty();
	markWorldDirty();
}

float ActorNode::y() const
{
	return m_Translation[1];
}

void ActorNode::y(float v)
{
	if(m_Translation[1] == v)
	{
		return;
	}
	m_Translation[1] = v;
	markDirty();
	markWorldDirty();
}

float ActorNode::scaleX() const
{
	return m_Scale[0];
}

void ActorNode::scaleX(float v)
{
	if(m_Scale[0] == v)
	{
		return;
	}
	m_Scale[0] = v;
	markDirty();
	markWorldDirty();
}

float ActorNode::scaleY() const
{
	return m_Scale[1];
}

void ActorNode::scaleY(float v)
{
	if(m_Scale[1] == v)
	{
		return;
	}
	m_Scale[1] = v;
	markDirty();
	markWorldDirty();
}

float ActorNode::rotation() const
{
	return m_Rotation;
}

void ActorNode::rotation(float v)
{
	if(m_Rotation == v)
	{
		return;
	}
	m_Rotation = v;
	markDirty();
	markWorldDirty();
}

float ActorNode::opacity() const
{
	return m_Opacity;
}

void ActorNode::opacity(float v)
{
	if(m_Opacity == v)
	{
		return;
	}
	m_Opacity = v;
	markWorldDirty();
}

float ActorNode::renderOpacity() const
{
	return m_RenderOpacity;
}

bool ActorNode::collapsedVisibility() const
{
	return m_IsCollapsedVisibility;
}

void ActorNode::collapsedVisibility(bool v)
{
	if(m_IsCollapsedVisibility != v)
	{
		m_IsCollapsedVisibility = v;
		markDirty();
		markWorldDirty();
	}
}

bool ActorNode::renderCollapsed() const
{
	return m_RenderCollapsed;
}

void ActorNode::markDirty()
{
	if(m_IsDirty)
	{
		return;
	}
	m_IsDirty = true;
}

void ActorNode::markWorldDirty()
{
	if(m_IsWorldDirty || m_SuppressMarkDirty)
	{
		return;
	}
	m_IsWorldDirty = true;
	for(ActorNode* node : m_Children)
	{
		node->markWorldDirty();
	}
	for(ActorNode* node : m_Dependents)
	{
		node->markWorldDirty();
	}
}

void ActorNode::addDependent(ActorNode* node)
{
	// Make sure it's not already a dependent.
	assert(std::find(m_Dependents.begin(), m_Dependents.end(), node) == m_Dependents.end());

	m_Dependents.push_back(node);
}

void ActorNode::removeDependent(ActorNode* node)
{
	auto itr = std::find(m_Dependents.begin(), m_Dependents.end(), node);
	if(itr == m_Dependents.end())
	{
		return;
	}
	m_Dependents.erase(itr);
}

Vec2D ActorNode::worldTranslation()
{
	Vec2D result;
	if(m_IsWorldDirty)
	{
		updateWorldTransform();
	}
	result[0] = m_WorldTransform[4];
	result[1] = m_WorldTransform[5];
	return result;
}

void ActorNode::worldTranslation(Vec2D& result)
{
	if(m_IsWorldDirty)
	{
		updateWorldTransform();
	}
	result[0] = m_WorldTransform[4];
	result[1] = m_WorldTransform[5];
}

void ActorNode::setRotationOverride(float v)
{
	if(!m_OverrideRotation || m_OverrideRotationValue != v)
	{
		m_OverrideRotation = true;
		m_OverrideRotationValue = v;
		markDirty();
		markWorldDirty();
	}
}

void ActorNode::clearRotationOverride()
{
	if(m_OverrideRotation)
	{
		m_OverrideRotation = false;
		markDirty();
		markWorldDirty();
	}
}

float ActorNode::overrideRotationValue() const
{
	return m_OverrideRotationValue;	
}

void ActorNode::updateTransform()
{
	m_IsDirty = false;

	Mat2D::fromRotation(m_Transform, m_OverrideRotation ? m_OverrideRotationValue : m_Rotation);
	m_Transform[4] = m_Translation[0];
	m_Transform[5] = m_Translation[1];
	Mat2D::scale(m_Transform, m_Transform, m_Scale);
}

void ActorNode::updateTransforms()
{
	if(m_IsDirty)
	{
		updateTransform();
	}
	if(m_IsWorldDirty)
	{
		updateWorldTransform();
	}
}

void ActorNode::updateWorldTransform()
{
	m_IsWorldDirty = false;

	if(m_IsDirty)
	{
		updateTransform();
	}

	m_RenderOpacity = m_Opacity;

	if(m_Parent != nullptr)
	{
		m_Parent->updateTransforms();

		bool isRenderCollapsed = (m_IsCollapsedVisibility || m_Parent->renderCollapsed());
		if(m_RenderCollapsed != isRenderCollapsed)
		{
			m_RenderCollapsed = isRenderCollapsed;
		}

		m_RenderOpacity *= m_Parent->renderOpacity();
		if(!m_OverrideWorldTransform)
		{
			Mat2D::multiply(m_WorldTransform, m_Parent->m_WorldTransform, m_Transform);
		}
	}
	else if(!m_OverrideWorldTransform)
	{
		Mat2D::copy(m_WorldTransform, m_Transform);
	}
}

void ActorNode::addChild(ActorNode* node)
{
	assert(node != nullptr);
	if(node->m_Parent != nullptr)
	{
		node->m_Parent->removeChild(node);
	}
	node->m_Parent = this;
	m_Children.push_back(node);
}

void ActorNode::removeChild(ActorNode* node)
{
	auto itr = std::find(m_Children.begin(), m_Children.end(), node);
	if(itr == m_Children.end())
	{
		return;
	}
	m_Children.erase(itr);
	node->m_Parent = nullptr;

}

void ActorNode::resolveComponentIndices(ActorComponent** components)
{
	Base::resolveComponentIndices(components);
}

ActorComponent* ActorNode::makeInstance(Actor* resetActor)
{
	ActorNode* instanceNode = new ActorNode();
	instanceNode->copy(this, resetActor);
	return instanceNode;
}

void ActorNode::copy(ActorNode* node, Actor* resetActor)
{
	Base::copy(node, resetActor);
	m_IsDirty = true;
	m_IsWorldDirty = true;
	Mat2D::copy(m_Transform, node->m_Transform);
	Mat2D::copy(m_WorldTransform, node->m_WorldTransform);
	Vec2D::copy(m_Translation, node->m_Translation);
	Vec2D::copy(m_Scale, node->m_Scale);
	m_Rotation = node->m_Rotation;
	m_Opacity = node->m_Opacity;
	m_RenderOpacity = node->m_RenderOpacity;
	m_OverrideWorldTransform = node->m_OverrideWorldTransform;
	m_OverrideRotation = node->m_OverrideRotation;
	m_OverrideRotationValue = node->m_OverrideRotationValue;
}

ActorNode* ActorNode::read(Actor* actor, BlockReader* reader, ActorNode* node)
{
	if(node == nullptr)
	{
		node = new ActorNode();
	}

	Base::read(actor, reader, node);

	reader->read(node->m_Translation);
	node->m_Rotation = reader->readFloat();
	reader->read(node->m_Scale);
	node->m_Opacity = reader->readFloat();
	node->m_IsCollapsedVisibility = (reader->readByte() == 1);

	return node;
}