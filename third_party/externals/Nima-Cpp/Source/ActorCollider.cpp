#include "ActorCollider.hpp"
#include "BlockReader.hpp"

using namespace nima;

ActorCollider::ActorCollider(ComponentType type) : 
	ActorNode(type),
	m_IsCollisionEnabled(true)
{
}

void ActorCollider::copy(ActorCollider* node, Actor* resetActor)
{
	Base::copy(node, resetActor);

	m_IsCollisionEnabled = node->m_IsCollisionEnabled;
}

ActorCollider* ActorCollider::read(Actor* actor, BlockReader* reader, ActorCollider* collider)
{
	Base::read(actor, reader, collider);
	collider->m_IsCollisionEnabled = reader->readByte() == 1;
	return collider;
}

bool ActorCollider::isCollisionEnabled() const
{
	return m_IsCollisionEnabled;
}

void ActorCollider::isCollisionEnabled(bool isIt)
{
	if(m_IsCollisionEnabled != isIt)
	{
		m_IsCollisionEnabled = isIt;
	}
}
// Rectangle

ActorColliderRectangle::ActorColliderRectangle() : 
	ActorCollider(ComponentType::ColliderRectangle),
	m_Width(0.0f),
	m_Height(0.0f)
{

}

ActorComponent* ActorColliderRectangle::makeInstance(Actor* resetActor)
{
	ActorColliderRectangle* instanceProp = new ActorColliderRectangle();
	instanceProp->copy(this, resetActor);
	return instanceProp;
}

void ActorColliderRectangle::copy(ActorColliderRectangle* collider, Actor* resetActor)
{
	Base::copy(collider, resetActor);
	m_Width = collider->m_Width;
	m_Height = collider->m_Height;
}

ActorColliderRectangle* ActorColliderRectangle::read(Actor* actor, BlockReader* reader, ActorColliderRectangle* collider)
{
	if(collider == nullptr)
	{
		collider = new ActorColliderRectangle();
	}
	Base::read(actor, reader, collider);
	collider->m_Width = reader->readFloat();
	collider->m_Height = reader->readFloat();
	return collider;
}

float ActorColliderRectangle::width() const
{
	return m_Width;
}

float ActorColliderRectangle::height() const
{
	return m_Height;
}


// Triangle

ActorColliderTriangle::ActorColliderTriangle() : 
	ActorCollider(ComponentType::ColliderTriangle),
	m_Width(0.0f),
	m_Height(0.0f)
{

}

ActorComponent* ActorColliderTriangle::makeInstance(Actor* resetActor)
{
	ActorColliderTriangle* instanceProp = new ActorColliderTriangle();
	instanceProp->copy(this, resetActor);
	return instanceProp;
}

void ActorColliderTriangle::copy(ActorColliderTriangle* collider, Actor* resetActor)
{
	Base::copy(collider, resetActor);
	m_Width = collider->m_Width;
	m_Height = collider->m_Height;
}

ActorColliderTriangle* ActorColliderTriangle::read(Actor* actor, BlockReader* reader, ActorColliderTriangle* collider)
{
	if(collider == nullptr)
	{
		collider = new ActorColliderTriangle();
	}
	Base::read(actor, reader, collider);
	collider->m_Width = reader->readFloat();
	collider->m_Height = reader->readFloat();
	return collider;
}

float ActorColliderTriangle::width() const
{
	return m_Width;
}

float ActorColliderTriangle::height() const
{
	return m_Height;
}

// Circle

ActorColliderCircle::ActorColliderCircle() : 
	ActorCollider(ComponentType::ColliderCircle),
	m_Radius(0.0f)
{

}

ActorComponent* ActorColliderCircle::makeInstance(Actor* resetActor)
{
	ActorColliderCircle* instanceProp = new ActorColliderCircle();
	instanceProp->copy(this, resetActor);
	return instanceProp;
}

void ActorColliderCircle::copy(ActorColliderCircle* collider, Actor* resetActor)
{
	Base::copy(collider, resetActor);
	m_Radius = collider->m_Radius;
}

ActorColliderCircle* ActorColliderCircle::read(Actor* actor, BlockReader* reader, ActorColliderCircle* collider)
{
	if(collider == nullptr)
	{
		collider = new ActorColliderCircle();
	}
	Base::read(actor, reader, collider);
	collider->m_Radius = reader->readFloat();
	return collider;
}

float ActorColliderCircle::radius() const
{
	return m_Radius;
}

// Polygon

ActorColliderPolygon::ActorColliderPolygon() : 
	ActorCollider(ComponentType::ColliderPolygon),
	m_IsInstance(false),
	m_ContourVertexCount(0),
	m_ContourVertices(nullptr)
{

}

ActorColliderPolygon::~ActorColliderPolygon()
{
	if(!m_IsInstance)
	{
		delete [] m_ContourVertices;
	}
}

ActorComponent* ActorColliderPolygon::makeInstance(Actor* resetActor)
{
	ActorColliderPolygon* instanceProp = new ActorColliderPolygon();
	instanceProp->copy(this, resetActor);
	return instanceProp;
}

void ActorColliderPolygon::copy(ActorColliderPolygon* collider, Actor* resetActor)
{
	Base::copy(collider, resetActor);
	m_IsInstance = true;
	m_ContourVertexCount = collider->m_ContourVertexCount;
	m_ContourVertices = collider->m_ContourVertices;
}

ActorColliderPolygon* ActorColliderPolygon::read(Actor* actor, BlockReader* reader, ActorColliderPolygon* collider)
{
	if(collider == nullptr)
	{
		collider = new ActorColliderPolygon();
	}
	Base::read(actor, reader, collider);

	collider->m_ContourVertexCount = reader->readUnsignedInt();
	collider->m_ContourVertices = new float[collider->m_ContourVertexCount*2];
	reader->readFloatArray(collider->m_ContourVertices, collider->m_ContourVertexCount*2);

	return collider;
}

float* ActorColliderPolygon::contourVertices() const
{
	return m_ContourVertices;
}

unsigned int ActorColliderPolygon::contourVertexCount() const
{
	return m_ContourVertexCount;
}

// Line

ActorColliderLine::ActorColliderLine() : 
	ActorCollider(ComponentType::ColliderLine),
	m_IsInstance(false),
	m_VertexCount(0),
	m_Vertices(nullptr)
{

}

ActorColliderLine::~ActorColliderLine()
{
	if(!m_IsInstance)
	{
		delete [] m_Vertices;
	}
}

ActorComponent* ActorColliderLine::makeInstance(Actor* resetActor)
{
	ActorColliderLine* instanceProp = new ActorColliderLine();
	instanceProp->copy(this, resetActor);
	return instanceProp;
}

void ActorColliderLine::copy(ActorColliderLine* collider, Actor* resetActor)
{
	Base::copy(collider, resetActor);
	m_IsInstance = true;
	m_VertexCount = collider->m_VertexCount;
	m_Vertices = collider->m_Vertices;
}

ActorColliderLine* ActorColliderLine::read(Actor* actor, BlockReader* reader, ActorColliderLine* collider)
{
	if(collider == nullptr)
	{
		collider = new ActorColliderLine();
	}
	Base::read(actor, reader, collider);

	collider->m_VertexCount = reader->readUnsignedInt();
	collider->m_Vertices = new float[collider->m_VertexCount*2];
	reader->readFloatArray(collider->m_Vertices, collider->m_VertexCount*2);

	return collider;
}

float* ActorColliderLine::vertices() const
{
	return m_Vertices;
}

unsigned int ActorColliderLine::vertexCount() const
{
	return m_VertexCount;
}