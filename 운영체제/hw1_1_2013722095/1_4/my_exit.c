#include <linux/kernel.h>

asmlinkage void sys_my_exit(void)
{
	printk("exit_[2013722095]\n");
}
