SRCS := hooking_test.c
obj-m := hooking.o

SYSCALL_ADDRESS = 0x$(subst R sys_call_table,,$(shell grep sys_call_table /boot/System.map-$(shell uname -r)))
CFLAGS_hooking.o += -DSYSCALL_TABLE=$(SYSCALL_ADDRESS)

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

run:default hooking_test
	sudo insmod hooking.ko
	./$(SRCS:.c=)
	sudo rmmod hooking
	./$(SRCS:.c=)


$(SRCS:.c=):$(SRCS:.c=.o)

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	$(RM) -rf hooking_test.o hooking_test