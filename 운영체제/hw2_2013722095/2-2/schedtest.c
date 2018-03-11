#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
#define MAX_PROCESSES 10000

int main ()
{
	FILE *fp;
	pid_t pid;
	struct timespec start, end;
	struct sched_param scheduler;
	long sec, nano;
	char filename[10];
	int i = 0;
	int value =0 ;
	clock_gettime(CLOCK_MONOTONIC, &start);
	scheduler.sched_priority = sched_get_priority_min(SCHED_OTHER);
	sched_setscheduler(0, SCHED_OTHER, &scheduler);
	chdir("./tmp2");


	while(i< MAX_PROCESSES)
	{
		pid = fork();
		if(pid< 0 )
			return -1;		// error case

		else if(pid == 0) 	// child process
		{
			sprintf(filename, "%d", i);		// read file
			fp = fopen(filename, "r");
			value = fgetc(fp);
			fclose(fp);
			if(i == 9999) break;		// escape loop when last process
			exit(0);						// close process
		}
							//parent process
		else	// kill parent when the last child is made
		{
			if(i == 9999)
				return 0;	
		}	


		i++;
	}
		
	clock_gettime(CLOCK_MONOTONIC, &end);
	sec = end.tv_sec - start.tv_sec;
	nano = end.tv_nsec - start.tv_nsec;
	if(nano < 0)
	{
		sec -=1;
		nano = 1- nano;
	}
	printf("\nConsumtion time : %ld.%ld\n", sec, nano);
	return 0;
}
