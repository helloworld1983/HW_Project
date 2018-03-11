#include <stdio.h>

#pragma warning(disable:4996)


float bisection(float x)
{
	float y = 0;

	y = (x*x*x) + 4 * (x*x) - 10;
	return y;
}

float absolute(float a, float b)
{
	float result = 0;
	result = a - b;
	if (result < 0)
		result = -result;

	return result;
}


void floating_point() {

	float x1 = 1;
	float x2 = 2;
	float x3 = 0;
	float tv = 0.001;

	while (0.5*absolute(x1, x2) >= tv)
	{

		x3 = (x1 + x2) / 2;

		if (bisection(x3)*bisection(x1) < 0)			// x1과 x3의 부호가 반대인가 ?
			x2 = x3;
		else											// 같은가
			x1 = x3;
	}

	printf("Result(Floating point) : (%f , %f)\n", x1, x2);
}


int main()
{
	floating_point();
}
