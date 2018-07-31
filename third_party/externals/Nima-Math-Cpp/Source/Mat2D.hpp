#ifndef _NIMA_MAT2D_HPP_
#define _NIMA_MAT2D_HPP_

#include <cstddef>

namespace nima
{
	class Vec2D;
	class Mat2D
	{
		private:
			float m_Buffer[6];

		public:
			Mat2D();
			Mat2D(const Mat2D& copy);
			
			inline const float* values() const { return m_Buffer; }

			float& operator[](std::size_t idx) { return m_Buffer[idx]; }
			const float& operator[](std::size_t idx) const { return m_Buffer[idx]; }

			static void fromRotation(Mat2D& result, float rad);
			static void scale(Mat2D& result, const Mat2D& mat, const Vec2D& vec);
			static void multiply(Mat2D& result, const Mat2D& a, const Mat2D& b);
			static bool invert(Mat2D& result, const Mat2D& a);
			static void copy(Mat2D& result, const Mat2D& a);
	};

	inline Mat2D operator*(const Mat2D& a, const Mat2D& b)
	{
		Mat2D result;
		Mat2D::multiply(result, a, b);
		return result;
	}
}
#endif