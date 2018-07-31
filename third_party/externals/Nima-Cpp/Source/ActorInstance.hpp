#ifndef _NIMA_ACTORINSTANCE_HPP_
#define _NIMA_ACTORINSTANCE_HPP_

#include "Actor.hpp"

namespace nima
{
	class ActorInstance : public Actor
	{
		public:
			ActorInstance(const Actor& actor);
			~ActorInstance();
	};
}
#endif