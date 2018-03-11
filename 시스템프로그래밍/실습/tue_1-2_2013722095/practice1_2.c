///////////////////////////////////////////////////////////////////////
// File Name : practice1_2.c					     //
// Date: 2017/03/30						     //
// Os: Ubuntu 12.04 LTS 64bits					     //
// Author: Choi Jae eun						     //
// Student ID: 2013722095					     //
// ------------------------------------------------------------------//
// Title: System Programming Assignment #1-2 ( ftp server ) 	     //
// Description :Example which activate like 'ls' command	     //
///////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
int main(int argc, char *argv[])
{	
	DIR *dp;				// Directory pointer
	struct dirent *dirp; 			// point inner object of directory
			
	if(argc >1 && (dp=opendir(argv[1])) == NULL) // Inserted path isn't exist path
		printf("<<< Cannot find that path >>>\n"); 
	     	// print error message and close directory, program


	else{	
		if(argc == 1) dp =opendir(".");  
		// if user didn't type path dp point current directory 

		while(dirp=readdir(dp)) // while read all file of current directory
			printf("%s\n", dirp->d_name); // print file's name
	}
	
	if(closedir(dp) != 0) // if failed to close directory, print error message
		printf("<<< Failed to close Directory >>>\n");
	return 0;
}

