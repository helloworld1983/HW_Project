#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/user.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#define RWMODE S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH 
#define EXMODE S_IRUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH 
#define ADD 1
#define MUL 2

uint8_t *Operation;
uint8_t *compiled_code;
int fd;

void sharedmem_init(); // access to shared memory
void sharedmem_exit();  
void drecompile_init(); // memory mapping start from shared memory
void drecompile_exit(); 
void* drecompile(uint8_t *func); // recompile part

int main(void)
{
	int (*func)(int a);
	long sec, nano;
	struct timespec start, end;

	clock_gettime(CLOCK_MONOTONIC, &start);



	sharedmem_init();
	drecompile_init();	
	func = (int (*)(int a))drecompile(Operation);
	printf("result : %d\n", func(1));
	drecompile_exit();
	sharedmem_exit();



	clock_gettime(CLOCK_MONOTONIC, &end);

	sec = end.tv_sec - start.tv_sec;
	nano = end.tv_nsec - start.tv_nsec;
	if(nano < 0)
	{
		sec -=1;
		nano = 1- nano;
	}
	printf("Excution time : %ld.%ld\n", sec, nano);

	return 0;
}


void sharedmem_init()
{
	int shm_id = shmget(1234, PAGE_SIZE, S_IRUSR | S_IWUSR);

	Operation = (char *)shmat(shm_id, NULL, 0);

}

void sharedmem_exit()
{
    shmdt(Operation);
    shmctl(shmget(1234, PAGE_SIZE, S_IRUSR | S_IWUSR), IPC_RMID, NULL);
}



void drecompile_init() 
{
	fd = open("my_recompile", O_RDWR|O_CREAT|O_TRUNC, RWMODE);

	compiled_code = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, fd, 0);
	assert(compiled_code != MAP_FAILED);
}

void drecompile_exit()
{
	msync(compiled_code, PAGE_SIZE, MS_SYNC);
	munmap(compiled_code, PAGE_SIZE);
}

void* drecompile(uint8_t* func)
{
	int add_count = 0;
	int add_function[3];

	int mul_count = 0;
	int mul_function[3];

	int i = 0;
	int j = 0;
	int k = 0;
	int mul_value = 0x00;

	while(1)
	{
		if(func[i] == 0x00)
		{
			close(fd);
			break;
		}

		compiled_code[j] = func[i];

#ifdef MYDY
		if(func[i] == 0x83) // found add inst
		{
			
			add_function[0] = func[i];	
			add_function[1] = func[i+1];
			add_function[2] = func[i+2];
			add_count = 1;
	

			while(1)
			{
				i+=3;
				if(func[i] != add_function[0] || func[i+1] != add_function[1] || func[i+2] != add_function[2])
					break;

				add_count++;
			}
			
			compiled_code[j] 	= add_function[0];
			compiled_code[j+1] 	= add_function[1];
			compiled_code[j+2] 	= add_function[2] * add_count;
			j += 3;
			continue;
		}

		else if(func[i] == 0x6b) // found add inst
		{
			mul_function[0] = func[i];	
			mul_function[1] = func[i+1];
			mul_function[2] = func[i+2];
			mul_count = 1;


			while(1)
			{
				i +=3;
				if(func[i] != mul_function[0] || func[i+1] != mul_function[1] || func[i+2] != mul_function[2])
					break;

				mul_count++;
			}


			compiled_code[j] = mul_function[0];
			compiled_code[j+1] = mul_function[1];

			mul_value = mul_function[2];
			for(k = 1; k < mul_count; k++)
				mul_value *=mul_function[2];
			
			

			compiled_code[j+2] = mul_value;

			j += 3;
			continue;
		}
#endif
		

		
		i++; 
		j++;

	}
	

	mprotect(compiled_code, PAGE_SIZE, PROT_READ | PROT_EXEC); // modify memory permission 
	return (void*)compiled_code;
}

