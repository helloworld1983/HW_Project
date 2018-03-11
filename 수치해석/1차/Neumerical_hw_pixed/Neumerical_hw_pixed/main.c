#include <stdio.h>
#include <math.h>
#pragma warning(disable:4996)
#define scale 512

int bisection(short int x)
{
	int y = 0;

	y = (x*x*x) + (4 * (x*x) * scale) - 10*scale*scale*scale;

	if (y < 0)
		return -1;
	else
		return 1;
}


void separation(short int x)
{
	int integer = 0;
	int fraction = 0;
	int value = 0;
	int a = 0;


	integer = x / scale;

	value = x % scale;

	for(; a<6 ; a++)
	{
		value = value * 10;
		fraction = fraction * 10;
		fraction = fraction + (value / scale);
		value = value % scale;
		if (value == 0)
			break;

	}

	printf("%d.%d", integer, fraction);
}

void fixed_point()
{

	short int x1 = 1;

	short int x2 = 2;

	short int x3 = 0;

	short int tv = 1;	 // short int ���� �ּҰ��� �ᱹ 1�̴�.

	
	x1 = x1 * scale;
	x2 = x2 * scale;

	while (0.5*abs(x1-x2) >= 1)
	{
		x3 = (x1 + x2) / 2;

		if (bisection(x3)*bisection(x1) < 0)			// x1�� x3�� ��ȣ�� �ݴ��ΰ� ?
			x2 = x3;
		else													// ������
			x1 = x3;
	}

	printf("Result(Pixed point) : (");	separation(x1);	printf(" , "); 	separation(x2); printf(")\n");
}


int main()
{
	fixed_point();
}
