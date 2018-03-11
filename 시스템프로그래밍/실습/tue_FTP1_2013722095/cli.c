/////////////////////////////////////////////////////////////////////
// File Name : cli.c						     //
// Date: 2017.04.28						     //
// Os: Ubuntu 12.04 LTS 64bits					     //
// Author: Choi Jae eun						     //
// Student ID: 2013722095					     //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #1		  	     //
// client part code 						     //
///////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
int main (int argc, char **argv) // recieve addtional argument
{
	char cmd[200];		// command array to pass into server
	char temp[100];		// temp array
	char oblist[20][20];	// object_list for directory
	char *token;	
	struct stat st;		// stat struct to check path is correct
	int a;		
	int ob_count=2;		// object_counter

	// save objects' name for commands which can recieve many arguments
	if((strcmp(argv[1], "mkdir") ==0) || (strcmp(argv[1], "rmdir") ==0) ||(strcmp(argv[1], "delete") ==0))
	{ 
	 while(1)
	  {
		if(argv[ob_count] == NULL) break; 
	   strcpy(oblist[ob_count], argv[ob_count]); ob_count++;}
	}


	if(strcmp(argv[1], "ls")==0) // if command is 'ls'
	{
		strcpy(cmd, "NLST "); // concatenate 'NLST' into cmd array
		if(argv[2] != NULL) // if input has option or path
		{
		// insert option first, path second
		  if(strcmp(argv[2], "-a")==0)   
		  {	strcat(cmd, "-a ");
	 		if(argv[3] != NULL) // cofirm that path is correct or not
			{
			   stat(argv[3], &st);
			   if(S_ISDIR(st.st_mode) | S_ISREG(st.st_mode)) strcat(cmd, argv[3]);
                           else {write(1, "error : wrong path\n", strlen("error : wrong path\n"));return 0;}
			}
 		  }

		// insert option first, path second
                  else if(strcmp(argv[2], "-l")==0)   
                  {   	strcat(cmd, "-l ");
                        if(argv[3] != NULL) 
                        { 
                           stat(argv[3], &st);
                           if(S_ISDIR(st.st_mode) | S_ISREG(st.st_mode))strcat(cmd, argv[3]); // confirm that path is correct or not
                           else {write(1, "error : wrong path\n", strlen("error : wrong path\n"));return 0;}
                        }

                  }
		// insert option first, path second and confirm that path is correct or not
                  else if((strcmp(argv[2], "-al")==0) || (strcmp(argv[2], "-la")==0)) 
                  {      strcat(cmd, "-al ");
                        if(argv[3] != NULL) 
                        { 
                           stat(argv[3], &st);
                           if(S_ISDIR(st.st_mode) | S_ISREG(st.st_mode))strcat(cmd, argv[3]);
                           else {write(1, "error : wrong path\n", strlen("error : wrong path\n"));return 0;}
                        }
               
		   }

		// insert path first option second. and confirm that path is correct or not
		 else if((argv[3] != NULL)&&(strcmp(argv[3], "-a")==0))
		  {
			strcat(cmd,"-a ");
			stat(argv[2], &st);
			if(S_ISDIR(st.st_mode) | S_ISREG(st.st_mode)) strcat(cmd, argv[2]);
			else{
			 write(1, "error : wrong path\n", strlen("error : wrong path\n"));
			return 0;}  
		}

		// insert path first, option second
                  else if((argv[3] !=NULL)&&(strcmp(argv[3], "-l")==0))
                  {
                        strcat(cmd,"-l ");
                        stat(argv[2], &st);
                        if(S_ISDIR(st.st_mode) | S_ISREG(st.st_mode)) strcat(cmd, argv[2]);
                        else {write(1, "error : wrong path\n", strlen("error : wrong path\n"));return 0;}

                  }

		// insert option second, path first
                  else if((argv[3] != NULL)&&((strcmp(argv[3], "-al")==0) || (strcmp(argv[3], "-la")==0)))
                  {
                        strcat(cmd,"-al ");
                        stat(argv[2], &st);
                        if(S_ISDIR(st.st_mode) | S_ISREG(st.st_mode)) strcat(cmd, argv[2]);
                        else{ write(1, "error : wrong path\n", strlen("error : wrong path\n"));return 0;}
                       
                  }
		  else // input has only path or unknown option
		  { 	
			stat(argv[2], &st);
			if(S_ISDIR(st.st_mode) | S_ISREG(st.st_mode)) // if argv[2] is openable type Thing
                            strcat(cmd, argv[2]);
			else // wrong path or unknown option
			 {write(1, "error : wrong path/option\n", strlen("error : wrong path/option\n")); return 0;}	
			       // if argv is not a wrong path and unknown option		
		  }
		}
	}

	else if(strcmp(argv[1], "dir")==0) // if command is dir, concatnate LIST into cmd
	{
		strcpy(cmd, "LIST ");
		if(argv[2] != NULL)
		{
			stat(argv[2], &st);
			if(S_ISDIR(st.st_mode) | S_ISREG(st.st_mode)) strcat(cmd, argv[2]);
	                else {write(1, "error : wrong path\n", strlen("error : wrong path\n"));return 0;}
	
		}
	}
	else if(strcmp(argv[1], "pwd")==0) // if command is pwd, concatnate PWD into cmd
	{
		strcpy(cmd, "PWD ");
	}
	else if(strcmp(argv[1], "cd")==0) // if command is cd
	{	
		if(argv[2] == NULL) // if has no arguments, error
		{ write(1,"error : Need Additional path\n", strlen("error : Need Additional path\n")); return 0;}
		else if(strcmp(argv[2],"..")==0) // if additional argument is '..', concatnate 'CDUP ..' into cmd array
			strcpy(cmd, "CDUP ..");
		else				// in other case, concatnate 'CWD' and argument
		{
			strcpy(cmd,"CWD ");
			strcat(cmd,argv[2]);
		}
	}
	else if(strcmp(argv[1],"mkdir")==0) // if command is mkdir
	{		
		if(argv[2] == NULL) // if has no argument, error
		{write(1,"error : Need more argument\n", strlen("error : Need more argument\n")); return 0;}
		else{
		 strcpy(cmd, "MKD "); // concatnate MKD and all additional arguments
		 for(a =2; a<ob_count ; a++)
		 {
			strcat(cmd, oblist[a]); strcat(cmd, " ");
		 }
		} 
	}
	else if(strcmp(argv[1], "delete")==0) // if command is delete
	{
                if(argv[2] == NULL) // if has no argument, error
                {write(1,"error : Need more argument\n", strlen("error : Need more argument\n")); return 0;}
                else{
                 strcpy(cmd, "DELE "); // concatnate DELE and additional arguments
                 for(a =2; a<ob_count ; a++)
                 {
                        strcat(cmd, oblist[a]); strcat(cmd, " ");
                 }
                }

	}
	else if(strcmp(argv[1], "rmdir")==0) // if command is rmdir
	{
                if(argv[2] == NULL) // if hasno argument, error
                {write(1,"error : Need more argument\n", strlen("error : Need more argument\n")); return 0;}
                else{ // concatnate RMD and additional argument
                 strcpy(cmd, "RMD "); 
                 for(a =2; a<ob_count ; a++)
                 {
                        strcat(cmd, oblist[a]); strcat(cmd, " ");
                 }
                }

	}
	else if(strcmp(argv[1], "rename")==0) // if command is rename
	{
		if((argv[2] == NULL) || (argv[3] == NULL)) // if input doesn't have two argument, error
		{write(1, "error : Need more argument\n", strlen("error : Need more argument\n"));return 0;}	
			// concatnate RNFR arg1 RNTO arg2
		strcpy(cmd, "RNFR ");
		strcat(cmd, argv[2]);
		strcat(cmd, " RNTO ");
		strcat(cmd, argv[3]);		
	}

	else if(strcmp(argv[1], "quit")==0) // if command is quit, copy QUIT into cmd array
	{
 	 	strcpy(cmd, "QUIT");
	}
	else{write(1, "error : Unknown Command...\n", strlen("error : Unknown Command...\n")); return 0;}	

	write(1, cmd, strlen(cmd)+1);
	return 0;

}
