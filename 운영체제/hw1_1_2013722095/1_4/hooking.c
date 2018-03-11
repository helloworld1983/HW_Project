#include <linux/module.h>
#include <linux/highmem.h>
#include <asm/unistd.h>

#define __NR_add 349
#define __NR_my_init 351
#define __NR_my_exit 352

void **syscall_table = (void**)SYSCALL_TABLE;

asmlinkage void (*backup)(void);

void make_rw(void *address)
{
   unsigned int level;
   pte_t *pte = lookup_address((unsigned long)address, &level);
   if(pte->pte &~ _PAGE_RW)
    pte->pte |= _PAGE_RW;
}
void make_ro(void *address)
{
   unsigned int level;
   pte_t *pte = lookup_address((unsigned long)address, &level);
   pte->pte = pte->pte &~ _PAGE_RW;
}


int hooking_start(void)
{
	make_rw(syscall_table);
	backup = syscall_table[__NR_my_exit];
	syscall_table[__NR_my_exit] = syscall_table[__NR_my_init];
	return 0;
}

void hooking_exit(void)
{
   syscall_table[__NR_my_exit] = backup;
   make_ro(syscall_table);
}

module_init(hooking_start);
module_exit(hooking_exit);
MODULE_LICENSE("GPL");
