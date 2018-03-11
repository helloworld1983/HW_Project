#include <linux/unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREAD 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int value[2];


pid_t gettid(){

	return syscall(__NR_gettid);
}

void* thread_sum1(void* arg)
{
	pthread_mutex_lock(&mutex);

	FILE *f1;
	FILE *f2;
	int  a, b, sum, i, j;
	
	char filename[3];

	chdir("./tmp1");		// move to file directory

	for(i = 0, j = 0; i < MAX_THREAD*2; i+=2, j++)
	{

		sprintf(filename, "%d", i);
		f1 = fopen(filename, "r");
		sprintf(filename, "%d", i+1);
		f2 = fopen(filename, "r");

		a = fgetc(f1);
		a -=48;
		b = fgetc(f2);
		b -=48;

		fclose(f1);
		fclose(f2);
		sum = a + b;

		printf("%d + %d = %d\n", a, b, sum);
		value[j] = sum;

				
	}

	pthread_mutex_unlock(&mutex);

	printf("PID : %ld \nTID : %ld\n\n", (unsigned long)getpid(), (unsigned long)gettid());
	return (void*)0;
}

void* thread_sum2(void* arg)
{
	
	int a ,b, i;

	
	pthread_mutex_lock(&mutex);

	a = value[0];
	b = value[1];


	pthread_mutex_unlock(&mutex);

	printf("%d + %d = %d\n", a, b, a+b);



	printf("PID: %ld\nTID: %ld\n", (unsigned long)getpid(),(unsigned long)gettid());
	return (void *)0;
}


int main(void)
{

	int state;
	pthread_t t1, t2;
	void *thread_result1, *thread_result2;

	pthread_create(&t1, NULL, thread_sum1, "thread1");
	usleep(400);
	pthread_create(&t2, NULL, thread_sum2, "thread2");
	pthread_join(t1, &thread_result1); // wait for t1's end
	pthread_join(t2, &thread_result2); // wait for t2's end

	pthread_mutex_destroy(&mutex);
	return 0;
}
