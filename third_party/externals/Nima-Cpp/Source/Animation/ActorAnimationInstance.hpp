#ifndef _NIMA_ACTORANIMATIONINSTANCE_HPP_
#define _NIMA_ACTORANIMATIONINSTANCE_HPP_

#include <string>
#include <vector>
#include "ActorAnimation.hpp"

namespace nima
{
	class ActorAnimationInstance
	{
		private:
			Actor* m_Actor;
			ActorAnimation* m_Animation;
			float m_Time;
			float m_Min;
			float m_Max;
			float m_Range;
			bool m_Loop;
			std::vector<ActorAnimationEvent> m_Events;
			void* m_EventCallbackUserData;
			ActorAnimationEvent::Callback m_EventCallback;

		public:

			ActorAnimationInstance(Actor* actor, ActorAnimation* animation);
			~ActorAnimationInstance();

			float duration() const;
			float min() const;
			float max() const;
			float time() const;
			void time(float value);
			bool isLooping() const;
			void isLooping(bool isIt);

			void advance(float seconds);
			void apply(float mix);

			void eventCallback(ActorAnimationEvent::Callback callback, void* userdata = nullptr);

	};
}
#endif