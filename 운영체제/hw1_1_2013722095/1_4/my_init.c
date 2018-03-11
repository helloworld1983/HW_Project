#include <linux/kernel.h>
asmlinkage void sys_my_init(void)
{
	printk("init_[2013722095]\n");
}
