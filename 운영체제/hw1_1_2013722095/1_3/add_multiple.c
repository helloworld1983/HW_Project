#include <stdio.h>
#include <linux/unistd.h>

int main(int argc, char ** argv)
{
	int a = 5;
	int b = 6;
	printf("%d + %d = %d\n", a, b, syscall(__NR_add, a, b));
	printf("%d * %d = %d\n", a, b, syscall(__NR_multiple, a, b));
}
