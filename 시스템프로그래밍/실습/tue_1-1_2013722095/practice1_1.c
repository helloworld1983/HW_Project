///////////////////////////////////////////////////////////////////////
// File Name : practice1_1.c					     //
// Date: 2017/03/25						     //
// Os: Ubuntu 12.04 LTS 64bits					     //
// Author: Choi Jae eun 					     //
// Student ID: 2013722095					     //
// ------------------------------------------------------------------//
// Title: System Programming Assignment #1-1 ( ftp server ) 	     //
// Description : Example for using getopt function		     //
///////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	int aflag= 0, bflag= 0;  // option a,b's flags
	char *cvalue= NULL;	 // pointer for string behind of option c
	int index, c;		 
	opterr= 0;		 // set opterr into 0, Don't print sys err message
	
	/* c recieve a token of string */
	/* Option C will recieve addition string  */ 
	while ((c = getopt(argc, argv, "abc:")) != -1)
	{
		switch (c)
		{ 
		case 'a':			// If option is 'a'
			aflag=1;		// set aflag into 1
			break;

		case 'b':			// If option is 'b'
			bflag=1;		// set bflag into 1
			break;	
		
		case 'c':			// If option is 'c'
			cvalue = optarg;	// 'cvalue'(pointer) point current c's argument
			break;
	
		case '?': 			// In  exceptional case
			printf("Unknown Option : %c\n",optopt);
			break; 
		}
	}
	
	printf("aflag = %d\nbflag = %d\ncvalue = %s\n", aflag, bflag, cvalue);
	
	for (index = optind; index < argc; index++) // Default option case
	           printf("Non-option argument %s\n", argv[index]);
	
	return 0;
}
