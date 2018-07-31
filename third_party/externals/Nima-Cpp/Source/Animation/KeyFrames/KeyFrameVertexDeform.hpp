#ifndef _NIMA_KEYFRAMEVERTEXDEFORM_HPP_
#define _NIMA_KEYFRAMEVERTEXDEFORM_HPP_

#include "KeyFrameWithInterpolation.hpp"

namespace nima
{
	class ActorComponent;

	class KeyFrameVertexDeform : public KeyFrameWithInterpolation
	{
		typedef KeyFrameWithInterpolation Base;
		private:
			float* m_Vertices;
			unsigned int m_VerticesCount;

		public:	
			KeyFrameVertexDeform();
			virtual ~KeyFrameVertexDeform();

			bool read(BlockReader* reader, ActorComponent* component) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
	};
}

#endif