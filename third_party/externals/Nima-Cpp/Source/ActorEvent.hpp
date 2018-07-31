#ifndef _NIMA_ACTOREVENT_HPP_
#define _NIMA_ACTOREVENT_HPP_

#include <string>
#include "ActorComponent.hpp"

namespace nima
{
	class ActorEvent : public ActorComponent
	{
		typedef ActorComponent Base;
		public:
			ActorEvent();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorComponent* node, Actor* resetActor);

			static ActorEvent* read(Actor* actor, BlockReader* reader, ActorEvent* component = NULL);
	};
}
#endif