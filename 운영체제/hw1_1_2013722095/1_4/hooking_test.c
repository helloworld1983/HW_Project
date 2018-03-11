#include <stdio.h>
#include <linux/unistd.h>
int main()
{
   syscall(352);
   return 0;
}
