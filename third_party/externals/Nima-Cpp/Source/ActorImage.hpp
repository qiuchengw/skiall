#ifndef _NIMA_ACTORIMAGE_HPP_
#define _NIMA_ACTORIMAGE_HPP_

#include "ActorRenderNode.hpp"
#include "BlendMode.hpp"

namespace nima
{
	class Actor;
	class BlockReader;
	class ActorNode;

	struct SequenceFrame
	{
		int m_AtlasIndex;
		int m_Offset;
	};

	class ActorImage : public ActorRenderNode
	{
		typedef ActorRenderNode Base;
		private:
			bool m_IsInstance;
			BlendMode m_BlendMode;
			int m_TextureIndex;
			float* m_Vertices;
			unsigned short* m_Triangles;
			int m_VertexCount;
			int m_TriangleCount;
			float* m_AnimationDeformedVertices;
			bool m_IsVertexDeformDirty;
			float* m_BoneMatrices;

			struct BoneConnection
			{
				unsigned short boneIndex;
				ActorNode* node;
				Mat2D bind;
				Mat2D ibind;

				BoneConnection();
			};

			int m_NumConnectedBones;
			BoneConnection* m_BoneConnections;

			SequenceFrame *m_SequenceFrames;
			float *m_SequenceUVs;
			int m_SequenceFrame;
			int m_SequenceFramesCount;

		  public:
			ActorImage();
			~ActorImage();
			ActorComponent* makeInstance(Actor* resetActor) override;
			void copy(ActorImage* node, Actor* resetActor);
			void resolveComponentIndices(ActorComponent** nodes) override;
			bool doesAnimationVertexDeform() const;
			void doesAnimationVertexDeform(bool doesIt);
			float* animationDeformedVertices();
			bool isVertexDeformDirty() const;
			void isVertexDeformDirty(bool isIt);
			int textureIndex() const;
			void disposeGeometry();
			int vertexCount() const;
			int triangleCount() const;
			int connectedBoneCount() const;
			int vertexStride() const;

			int boneInfluenceMatricesLength();
			float* boneInfluenceMatrices();

			float* vertices() const;
			unsigned short* triangles() const;
			BlendMode blendMode() const;

			int sequenceFramesCount() const;
			SequenceFrame* sequenceFrames() const;
			float* sequenceUVs() const;
			void sequenceFrame(int value);
			int sequenceFrame() const;
			
			static ActorImage* read(Actor* actor, BlockReader* reader, ActorImage* node = NULL);
			static ActorImage* readImageSequence(Actor* actor, BlockReader* reader, ActorImage* node = NULL);
	};
}
#endif