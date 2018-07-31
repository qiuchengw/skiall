#ifndef _NIMA_PROPERTYANIMATION_HPP_
#define _NIMA_PROPERTYANIMATION_HPP_

#include <string>

namespace nima
{
	class BlockReader;
	class ActorComponent;
	class KeyFrame;
	enum class PropertyType
	{
		Unknown = 0,
		PosX = 1,
		PosY = 2,
		ScaleX = 3,
		ScaleY = 4,
		Rotation = 5,
		Opacity = 6,
		DrawOrder = 7,
		Length = 8,
		VertexDeform = 9,
		IKStrength = 10,
		Trigger = 11,
		IntProperty = 12,
		FloatProperty = 13,
		StringProperty = 14,
		BooleanProperty = 15,
		IsCollisionEnabled = 16,
		Sequence = 17,
		ActiveChildIndex = 18,
		Max
	};

	class PropertyAnimation
	{
		private:
			PropertyType m_Type;
			KeyFrame** m_KeyFrames;
			int m_KeyFramesCount;

		public:

			PropertyAnimation();
			~PropertyAnimation();

			int keyFramesCount() const;
			inline const KeyFrame* keyFrame(int idx) const
			{
				return m_KeyFrames[idx];
			}

			PropertyType type() const;
			void read(BlockReader* reader, ActorComponent* component);
			void apply(float time, ActorComponent* component, float mix);

	};
}
#endif