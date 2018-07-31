#ifndef _NIMA_ACTORCOLLIDER_HPP_
#define _NIMA_ACTORCOLLIDER_HPP_

#include <string>
#include "ActorNode.hpp"

namespace nima
{
	class ActorCollider : public ActorNode
	{
		typedef ActorNode Base;
		private:
			bool m_IsCollisionEnabled;
			
		public:
			ActorCollider(ComponentType type);
			void copy(ActorCollider* node, Actor* resetActor);
			static ActorCollider* read(Actor* actor, BlockReader* reader, ActorCollider* collider);

			bool isCollisionEnabled() const;
			void isCollisionEnabled(bool isIt);
	};

	class ActorColliderRectangle : public ActorCollider
	{
		typedef ActorCollider Base;
		private:
			float m_Width;
			float m_Height;
			
		public:
			ActorColliderRectangle();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorColliderRectangle* collider, Actor* resetActor);
			static ActorColliderRectangle* read(Actor* actor, BlockReader* reader, ActorColliderRectangle* collider = NULL);

			float width() const;
			float height() const;
	};

	class ActorColliderTriangle : public ActorCollider
	{
		typedef ActorCollider Base;
		private:
			float m_Width;
			float m_Height;
			
		public:
			ActorColliderTriangle();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorColliderTriangle* collider, Actor* resetActor);
			static ActorColliderTriangle* read(Actor* actor, BlockReader* reader, ActorColliderTriangle* collider = NULL);

			float width() const;
			float height() const;
	};

	class ActorColliderCircle : public ActorCollider
	{
		typedef ActorCollider Base;
		private:
			float m_Radius;
			
		public:
			ActorColliderCircle();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorColliderCircle* collider, Actor* resetActor);
			static ActorColliderCircle* read(Actor* actor, BlockReader* reader, ActorColliderCircle* collider = NULL);

			float radius() const;
	};

	class ActorColliderPolygon : public ActorCollider
	{
		typedef ActorCollider Base;
		private:
			bool m_IsInstance;
			unsigned int m_ContourVertexCount;
			float* m_ContourVertices;
			
		public:
			ActorColliderPolygon();
			~ActorColliderPolygon();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorColliderPolygon* collider, Actor* resetActor);
			static ActorColliderPolygon* read(Actor* actor, BlockReader* reader, ActorColliderPolygon* collider = NULL);

			float* contourVertices() const;
			unsigned int contourVertexCount() const;
	};

	class ActorColliderLine : public ActorCollider
	{
		typedef ActorCollider Base;
		private:
			bool m_IsInstance;
			unsigned int m_VertexCount;
			float* m_Vertices;
			
		public:
			ActorColliderLine();
			~ActorColliderLine();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorColliderLine* collider, Actor* resetActor);
			static ActorColliderLine* read(Actor* actor, BlockReader* reader, ActorColliderLine* collider = NULL);

			float* vertices() const;
			unsigned int vertexCount() const;
	};
}
#endif