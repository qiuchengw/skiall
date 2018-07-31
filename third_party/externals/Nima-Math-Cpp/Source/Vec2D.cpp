#include "Vec2D.hpp"
#include "Mat2D.hpp"
#include <cmath>

using namespace nima;

Vec2D::Vec2D() : m_Buffer{0.0f,0.0f}
{

}

Vec2D::Vec2D(float x, float y) : m_Buffer{x, y}
{

}

Vec2D::Vec2D(const Vec2D& copy) : m_Buffer{copy.m_Buffer[0], copy.m_Buffer[1]}
{

}

void Vec2D::transform(Vec2D& result, const Vec2D& a, const Mat2D& m)
{
	float x = a[0];
	float y = a[1];
	result[0] = m[0] * x + m[2] * y + m[4];
	result[1] = m[1] * x + m[3] * y + m[5];
}

void Vec2D::add(Vec2D& result, const Vec2D& a, const Vec2D& b)
{
	result[0] = a[0] + b[0];
	result[1] = a[1] + b[1];
}

void Vec2D::subtract(Vec2D& result, const Vec2D& a, const Vec2D& b)
{
	result[0] = a[0] - b[0];
	result[1] = a[1] - b[1];
}

float Vec2D::length(const Vec2D& a)
{
	float x = a[0];
	float y = a[1];
	return sqrt(x*x + y*y);
}

void Vec2D::copy(Vec2D& result, const Vec2D& a)
{
	result[0] = a[0];
	result[1] = a[1];
}

void Vec2D::normalize(Vec2D& result, const Vec2D& a)
{
	float x = a[0];
	float y = a[1];
	float len = x*x + y*y;
	if (len > 0.0f) 
	{
		len = 1.0f / std::sqrt(len);
		result[0] = a[0] * len;
		result[1] = a[1] * len;
    }
}

float Vec2D::dot(const Vec2D& a, const Vec2D& b)
{
	return a[0] * b[0] + a[1] * b[1];
}