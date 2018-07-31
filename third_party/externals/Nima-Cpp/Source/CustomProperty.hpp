#ifndef _NIMA_CUSTOMPROPERTY_HPP_
#define _NIMA_CUSTOMPROPERTY_HPP_

#include <string>
#include "ActorComponent.hpp"

namespace nima
{
	class CustomIntProperty : public ActorComponent
	{
		typedef ActorComponent Base;
		private:
			int m_Value;
			
		public:
			CustomIntProperty();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(CustomIntProperty* property, Actor* resetActor);
			static CustomIntProperty* read(Actor* actor, BlockReader* reader, CustomIntProperty* property = NULL);

			void resolveComponentIndices(ActorComponent** components) override;
			int value() const;
			void value(int v);

	};
	class CustomFloatProperty : public ActorComponent
	{
		typedef ActorComponent Base;
		private:
			float m_Value;
			
		public:
			CustomFloatProperty();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(CustomFloatProperty* property, Actor* resetActor);
			static CustomFloatProperty* read(Actor* actor, BlockReader* reader, CustomFloatProperty* property = NULL);

			void resolveComponentIndices(ActorComponent** components) override;
			float value() const;
			void value(float v);
	};
	class CustomStringProperty : public ActorComponent
	{
		typedef ActorComponent Base;
		private:
			std::string m_Value;
			
		public:
			CustomStringProperty();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(CustomStringProperty* property, Actor* resetActor);
			static CustomStringProperty* read(Actor* actor, BlockReader* reader, CustomStringProperty* property = NULL);

			void resolveComponentIndices(ActorComponent** components) override;
			const std::string& value() const;
			void value(const std::string& v);
	};
	class CustomBooleanProperty : public ActorComponent
	{
		typedef ActorComponent Base;
		private:
			bool m_Value;
			
		public:
			CustomBooleanProperty();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(CustomBooleanProperty* property, Actor* resetActor);
			static CustomBooleanProperty* read(Actor* actor, BlockReader* reader, CustomBooleanProperty* property = NULL);

			void resolveComponentIndices(ActorComponent** components) override;
			const bool value() const;
			void value(const bool v);
	};
}
#endif