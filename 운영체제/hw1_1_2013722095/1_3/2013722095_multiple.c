#include <linux/kernel.h>
asmlinkage int sys_multiple(int a, int b)
{
	return a*b;
}
