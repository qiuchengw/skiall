#ifndef _NIMA_ACTORCOMPONENT_HPP_
#define _NIMA_ACTORCOMPONENT_HPP_

#include <string>
#include <vector>
#include <nima/Mat2D.hpp>
#include <nima/Vec2D.hpp>

namespace nima
{
	class Actor;
	class ActorNode;
	class BlockReader;
	class CustomIntProperty;
	class CustomFloatProperty;
	class CustomStringProperty;
	class CustomBooleanProperty;

	enum class ComponentType
	{
		ActorNode = 2,
		ActorBone = 3,
		ActorRootBone = 4,
		ActorImage = 5,
		ActorIKTarget = 11,
		ActorEvent = 12,
		CustomIntProperty = 13,
		CustomFloatProperty = 14,
		CustomStringProperty = 15,
		CustomBooleanProperty = 16,
		ColliderRectangle = 17, // TODO
		ColliderTriangle = 18, // TODO
		ColliderCircle = 19, // TODO
		ColliderPolygon = 20, // TODO
		ColliderLine = 21, // TODO
		ActorNodeSolo = 23,
		NestedActorNode = 24,
		ActorStaticMesh = 27
	};


	class ActorComponent
	{
		protected:
			ComponentType m_Type;
			std::string m_Name;
			ActorNode* m_Parent;
			Actor* m_Actor;
			std::vector<CustomIntProperty*> m_CustomIntProperties;
			std::vector<CustomFloatProperty*> m_CustomFloatProperties;
			std::vector<CustomStringProperty*> m_CustomStringProperties;
			std::vector<CustomBooleanProperty*> m_CustomBooleanProperties;

		private:
			unsigned short m_ParentIdx;
			unsigned short m_Idx;
			
		protected:
			ActorComponent(ComponentType type);
			ActorComponent(Actor* actor, ComponentType type);

		public:
			virtual ~ActorComponent();
			Actor* actor() const;
			const std::string& name() const;
			ComponentType type() const;

			ActorNode* parent() const;
			unsigned short parentIdx() const;
			unsigned short idx() const;
			virtual void resolveComponentIndices(ActorComponent** components);
			virtual ActorComponent* makeInstance(Actor* resetActor) = 0;
			void copy(ActorComponent* node, Actor* resetActor);
			virtual bool isNode() { return false; }

			static ActorComponent* read(Actor* actor, BlockReader* reader, ActorComponent* component = NULL);

			void addCustomIntProperty(CustomIntProperty* property);
			void addCustomFloatProperty(CustomFloatProperty* property);
			void addCustomStringProperty(CustomStringProperty* property);
			void addCustomBooleanProperty(CustomBooleanProperty* property);

			CustomIntProperty* getCustomIntProperty(const std::string& name);
			CustomFloatProperty* getCustomFloatProperty(const std::string& name);
			CustomStringProperty* getCustomStringProperty(const std::string& name);
			CustomBooleanProperty* getCustomBooleanProperty(const std::string& name);
	};
}
#endif