#ifndef _NIMA_ACTORSTATICMESH_HPP_
#define _NIMA_ACTORSTATICMESH_HPP_

#include "ActorRenderNode.hpp"
#include "BlendMode.hpp"

namespace nima
{
	class Actor;
	class BlockReader;
	class ActorNode;

	class ActorStaticMesh : public ActorRenderNode
	{
		typedef ActorRenderNode Base;
		private:
			bool m_IsInstance;
			BlendMode m_BlendMode;

		public:
			struct Surface
			{
				int textureIndex;
				unsigned int vertexCount;
				unsigned int triangleCount;
				float* vertices;
				unsigned short* triangles;

				Surface();
				virtual ~Surface();
				virtual void disposeGeometry();
			};

		protected:
			int m_NumSurfaces;
			Surface** m_Surfaces;

			virtual Surface* makeSurface();

		public:
			ActorStaticMesh();
			~ActorStaticMesh();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorStaticMesh* node, Actor* resetActor);
			void resolveComponentIndices(ActorComponent** nodes) override;
			virtual void disposeGeometry();
			BlendMode blendMode() const;
			int numSurfaces() const;
			Surface* surface(int index);

			static ActorStaticMesh* read(Actor* actor, BlockReader* reader, ActorStaticMesh* node = NULL);
	};
}
#endif