#ifndef _NIMA_ACTOR_HPP_
#define _NIMA_ACTOR_HPP_

#include "ActorNode.hpp"
#include "ActorImage.hpp"
#include "ActorStaticMesh.hpp"
#include "BlockReader.hpp"
#include "Solver.hpp"
#include "Animation/ActorAnimation.hpp"
#include "Animation/ActorAnimationInstance.hpp"
#include "NestedActorAsset.hpp"
#include "NestedActorNode.hpp"

namespace nima
{
	enum class BlockType
	{
		Unknown = 0,
		Components = 1,
		ActorNode = 2,
		ActorBone = 3,
		ActorRootBone = 4,
		ActorImage = 5,
		View = 6,
		Animation = 7,
		Animations = 8,
		Atlases = 9,
		Atlas = 10,
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
		ActorImageSequence = 22, // TODO
		ActorNodeSolo = 23,
		NestedActorNode = 24,
		NestedActorAssets = 25,
		NestedActorAsset = 26,
		ActorStaticMesh = 27
	};

	class Actor
	{
		friend class ActorAnimationInstance;
		friend class NestedActorNode;
		friend class ActorImage;
		friend class ActorStaticMesh;

		public:
			Actor();
			virtual ~Actor();

			enum Flags
			{
				IsDrawOrderDirty = 1<<0,
				IsVertexDeformDirty = 1<<1,
				IsInstance = 1<<2
			};
		private:
			unsigned short m_Flags;
			unsigned int m_ComponentCount;
			unsigned int m_NodeCount;
			ActorComponent** m_Components;
			ActorNode** m_Nodes;
			ActorNode* m_Root;
			void* m_EventCallbackUserData;
			ActorAnimationEvent::Callback m_EventCallback;
			void readComponentsBlock(BlockReader* block);
			void readAnimationsBlock(BlockReader* block);
			void readNestedActorAssetsBlock(BlockReader* block);
			
		protected:
			int m_MaxTextureIndex;
			unsigned int m_ImageNodeCount;
			unsigned int m_RenderNodeCount;
			unsigned int m_SolverNodeCount;
			unsigned int m_AnimationsCount;
			unsigned int m_NestedActorAssetCount;
			unsigned int m_NestedActorNodeCount;
			std::string m_BaseFilename;

			ActorImage** m_ImageNodes;
			ActorRenderNode** m_RenderNodes;
			Solver** m_Solvers;
			ActorAnimation* m_Animations;
			NestedActorAsset** m_NestedActorAssets;
			NestedActorNode** m_NestedActorNodes;

			virtual ActorImage* makeImageNode();
			virtual ActorStaticMesh* makeStaticMeshNode();
			virtual NestedActorNode* makeNestedActorNode();
			virtual NestedActorAsset* makeNestedActorAsset();
			virtual void dispose();
			virtual void updateVertexDeform(ActorImage* image) {}

		public:
			void load(unsigned char* bytes, unsigned int length);
			void load(const std::string& filename);
			void setFilename(const std::string& filename);

			unsigned int componentCount() const;
			ActorComponent* component(unsigned int index) const;
			ActorComponent* component(unsigned short index) const;
			ActorComponent* component(const std::string& name) const;

			const int nestedActorCount() const;
			NestedActorAsset* nestedActorAsset(unsigned int index) const;

			void eventCallback(ActorAnimationEvent::Callback callback, void* userdata = nullptr);
			
			template<typename T>
			T component(const std::string& name) const
			{
				return dynamic_cast<T>(component(name));
			}

			ActorNode* root() const;
			ActorAnimation* animation(const std::string& name) const;
			ActorAnimationInstance* animationInstance(const std::string& name);

			void copy(const Actor& actor);
			const int textureCount() const;
			const std::string& baseFilename() const;
			virtual void advance(float elapsedSeconds);
			void markDrawOrderDirty();

			virtual Actor* makeInstance() const;

			template<typename T>
			T* instance() const
			{
				return dynamic_cast<T*>(makeInstance());
			}
	};
}
#endif