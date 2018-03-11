///////////////////////////////////////////////////////////////////////
// File Name : practice1_3.c					     //
// Date: 2017/04/11						     //
// Os: Ubuntu 12.04 LTS 64bits					     //
// Author: Choi Jae eun						     //
// Student ID: 2013722095					     //
// ------------------------------------------------------------------//
// Title: System Programming Assignment #1-3 ( ftp server ) 	     //
// Description :Example for making other own command 		     //
///////////////////////////////////////////////////////////////////////



#include <stdio.h>
#include <unistd.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////
// Main function						      //
// =================================================================  //
// Input: argc, **argv -> arguments' count, string behind of exec file//
// char   getStr  	-> array for command string recieving	      //
// char*  token, token2 -> point argument inside of getStr string     // 
// int    errflag 	-> flag for error case (set : 1 / none :0)    //
//Output: int-0 success						      //
//0 fail							      //
// Purpose: convert existing command into your own command	      //
////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{
	char getStr[128];
	char* token;
	char* token2;
	int errflag;
	
	while(1)
	{
		printf("[com] ");
		errflag = 0;
		gets(getStr);
		token = strtok(getStr, " "); 
		if(strcmp(token, "pwd")==0)				// input command is pwd
		{
			if(strtok(NULL," ")!=NULL) errflag = 1;		// if string has additional argument, set errflag into 1
			else printf("%s\n", getcwd(getStr,128));	// print current working directory's path
		}
		else if(strcmp(token, "cd")==0)				// input command is cd
		{
			if((token = strtok(NULL," "))==NULL) errflag = 1;// if string has no additional argument, set errflag into 1 
			else{if(chdir(token)!=0) errflag = 1;}		 // if input path(token) isn't exist. set errflag into 1
		}
		else if(strcmp(token, "mkdir")==0)			// input command is mkdir
		{
			token = strtok(NULL, " ");			// get two token
			token2 = strtok(NULL," ");
			umask(0);					// set permission into 000
			if(token == NULL || token2 !=NULL)		// if string has no arguments or has second argument
				errflag = 1;				// set error flag into 1
			else{if(mkdir(token,0777)!= 0) errflag = 1;}	// when u make directory, set its permission into 777 and if couldn't make directory, set errflag into 1
		}
		else if(strcmp(token, "rmdir")==0)			// inpuuuuut command is rmdir
		{
			if((token = strtok(NULL," ")) == NULL) 		// if string has no additional argument set errflag into 1
				errflag = 1;
			else{if(rmdir(token)!= 0)errflag = 1;}		// if couldn't find token named directory, set errflag into 1
		}
		else if(strcmp(token, "rename")==0)			// input command is rename
		{
			token = strtok(NULL, " ");			// get two token
			token2 = strtok(NULL," ");
			if(token == NULL || token2 == NULL)		// string has no additional arguments
			errflag = 1;					// set  errflag into 1
			else {if(rename(token, token2) != 0)		// if counldn't find token named file or change its name, set error errflag into 1
			errflag =1;}
		}
		else if(strcmp(token, "exit")==0) break;		// input command is exit, escape loop
		else {printf("Error:Uknown Command\n");}		// Unknown command error
		
		if(errflag) printf("Error:re-check commands\n");	// if errflag is set, print error sentence
	
	}
	return 0;
}
