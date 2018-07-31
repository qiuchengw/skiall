#ifndef _NIMA_ACTORBONE_HPP_
#define _NIMA_ACTORBONE_HPP_

#include "ActorNode.hpp"

#include <nima/Vec2D.hpp>

namespace nima
{
	class Actor;
	class BlockReader;
	class ActorNode;

	class ActorBone : public ActorNode
	{
		typedef ActorNode Base;
		protected:
			float m_Length;
			bool m_IsConnectedToImage;

		public:
			ActorBone();
			float length() const;
			void length(float l);
			bool isConnectedToImage() const;
			void isConnectedToImage(bool isIt);
			void tipWorldTranslation(Vec2D& result);
			Vec2D tipWorldTranslation();

			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorBone* node, Actor* resetActor);

			static ActorBone* read(Actor* actor, BlockReader* reader, ActorBone* node = NULL);
	};
}
#endif