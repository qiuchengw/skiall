#ifndef _NIMA_ACTORNODE_HPP_
#define _NIMA_ACTORNODE_HPP_

#include <vector>
#include <nima/Mat2D.hpp>
#include <nima/Vec2D.hpp>
#include "ActorComponent.hpp"

namespace nima
{
	class Actor;
	class BlockReader;

	class ActorNode : public ActorComponent
	{
		typedef ActorComponent Base;
		protected:
			std::vector<ActorNode*> m_Children;
			std::vector<ActorNode*> m_Dependents;
			Mat2D m_Transform;
			Mat2D m_WorldTransform;
			Vec2D m_Translation;
			float m_Rotation;
			Vec2D m_Scale;
			float m_Opacity;
			float m_RenderOpacity;

		private:
			bool m_IsDirty;
			bool m_IsWorldDirty;
			bool m_SuppressMarkDirty;
			bool m_OverrideWorldTransform;
			bool m_OverrideRotation;
			bool m_IsCollapsedVisibility;
			bool m_RenderCollapsed;
			float m_OverrideRotationValue;

		protected:
			ActorNode(ComponentType type);
		private:
			ActorNode(Actor* actor, ComponentType type);
		public:
			ActorNode(Actor* actor);
			ActorNode();
			virtual ~ActorNode();
			bool suppressMarkDirty() const;
			void suppressMarkDirty(bool suppress); 
			bool isWorldDirty() const;
			bool isDirty() const;
			const Mat2D& transform();
			const Mat2D& worldTransform();
			void overrideWorldTransform(const Mat2D& transform);
			void clearWorldTransformOverride();

			float x() const;
			void x(float v);
			float y() const;
			void y(float v);
			float scaleX() const;
			void scaleX(float v);
			float scaleY() const;
			void scaleY(float v);
			float rotation() const;
			void rotation(float v);
			float opacity() const;
			void opacity(float v);
			float renderOpacity() const;
			bool collapsedVisibility() const;
			void collapsedVisibility(bool v);
			bool renderCollapsed() const;
			void markDirty();
			void markWorldDirty();
			void addDependent(ActorNode* node);
			void removeDependent(ActorNode* node);
			void worldTranslation(Vec2D& result);
			Vec2D worldTranslation();
			void setRotationOverride(float v);
			void clearRotationOverride();
			float overrideRotationValue() const;
			void updateTransform();
			void updateTransforms();
			virtual void updateWorldTransform();
			void addChild(ActorNode* node);
			void removeChild(ActorNode* node);
			void resolveComponentIndices(ActorComponent** components) override;
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorNode* node, Actor* resetActor);
			bool isNode() override { return true; }

			static ActorNode* read(Actor* actor, BlockReader* reader, ActorNode* node = NULL);
	};
}
#endif