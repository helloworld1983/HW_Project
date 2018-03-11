#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 10000

void main()
{
	int i;
	char filename[5];
	mkdir("./tmp2", 0755);					// file directory
	chdir("./tmp2");
	srand(time(NULL));

	for(i = 0; i < MAX_PROCESSES; i ++)
	{
		sprintf(filename, "%d", i);		// filename change

		FILE *f_write;
		f_write = fopen(filename, "w");	// make file
		fprintf(f_write, "%d", (rand()%9) + 1);	// write random value
		fclose(f_write);
	}
}
