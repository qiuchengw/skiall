#include <stdio.h>

#include <nima/Mat2D.hpp>
#include <nima/Vec2D.hpp>

using namespace nima;

int main(int argc, char** argv)
{
	printf("OK\n");
	Mat2D matA;
	matA[0] = 1.0f; matA[1] = 2.0f; matA[2] = 3.0f; matA[3] = 4.0f; matA[4] = 5.0f; matA[5] = 6.0f;
	Mat2D matB;
	matB[0] = 7.0f; matB[1] = 8.0f; matB[2] = 9.0f; matB[3] = 10.0f; matB[4] = 11.0f; matB[5] = 12.0f;

	Mat2D result;
	Mat2D::invert(result, matA);
	if(result[0] == -2.0f && result[1] == 1.0f && result[2] == 1.5f && result[3] == -0.5f && result[4] == 1.0f && result[5] == -2.0f)
	{
		printf("Invert ok\n");
	}

	Mat2D::multiply(result, matA, matB);
	if(result[0] == 31.0f && result[1] == 46.0f && result[2] == 39.0f && result[3] == 58.0f && result[4] == 52.0f && result[5] == 76.0f)
	{
		printf("Multiply ok\n");
	}

	return 0;
}