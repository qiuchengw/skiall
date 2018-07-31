#ifndef _NIMA_COLOR_HPP_
#define _NIMA_COLOR_HPP_

#include <cstddef>

namespace nima
{
	class Color
	{
		private:
			float m_Buffer[4];
		public:
			Color() : m_Buffer{1.0f, 1.0f, 1.0f, 1.0f}
			{

			}

			Color(float r, float g, float b) : m_Buffer{r, g, b, 1.0f}
			{

			}

			Color(float r, float g, float b, float a) : m_Buffer{r, g, b, a}
			{

			}

			Color(int r, int g, int b, int a) : m_Buffer{r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f}
			{

			}

			Color(int r, int g, int b) : m_Buffer{r / 255.0f, g / 255.0f, b / 255.0f, 1.0f}
			{

			}

			Color(float rgba) : m_Buffer{rgba, rgba, rgba, rgba}
			{

			}

			inline const float* values() const
			{
				return m_Buffer;
			}

			float& operator[](std::size_t idx) { return m_Buffer[idx]; }
			const float& operator[](std::size_t idx) const { return m_Buffer[idx]; }

			void set(float r, float g, float b, float a)
			{
				m_Buffer[0] = r;
				m_Buffer[1] = g;
				m_Buffer[2] = b;
				m_Buffer[3] = a;
			}

			void lerp(const Color& second, float amount)
			{
				float iamount = 1.0f - amount;
				for (int i = 0; i < 4; i++)
				{
					m_Buffer[i] = second.m_Buffer[i] * amount + m_Buffer[i] * iamount;
				}
			}

			void multiply(const Color& second)
			{
				for (int i = 0; i < 4; i++)
				{
					m_Buffer[i] *= second.m_Buffer[i];
				}
			}

			void red(float r) { m_Buffer[0] = r; }
			void green(float r) { m_Buffer[1] = r; }
			void blue(float r) { m_Buffer[2] = r; }
			void alpha(float r) { m_Buffer[3] = r; }

			float red() const { return m_Buffer[0]; }
			float green() const { return m_Buffer[1]; }
			float blue() const { return m_Buffer[2]; }
			float alpha() const { return m_Buffer[3]; }

			float r() const { return m_Buffer[0]; }
			float g() const { return m_Buffer[1]; }
			float b() const { return m_Buffer[2]; }
			float a() const { return m_Buffer[3]; }
	};
}
#endif