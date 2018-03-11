//////////////////////////////////////////////////////////////////////////
// File Name : cli.c													//
// Date: 2017.05.16														//
// Os: Ubuntu 12.04 LTS 64bits											//
// Author: CHoi Jae eun													//
// Student ID: 2013722095 												//
// -----------------------------------------------------------------	//
// Title: System Programming Assignment #2 (ftp server) 				//
// Description : server part code										//
//////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <signal.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#define MAX 4096
int sock_fd;
int convcomm(char* ,char* );
void sh_int(int); 
void sh_term(int);


int main (int argc, char **argv) // recieve addtional argument
{
	// main variable
	char cmd[MAX];		// input from user
	char CMD[MAX]; 		// converted command
	char result[MAX];	// result from server
	char client_pid[20];
	int n;

	// socket variable
	struct sockaddr_in serv_addr;

	signal(SIGINT, sh_int); // SIGINT handler
	signal(SIGTERM, sh_term);

	/* confirm arguments */
	if(argc != 3)
	{
		write(STDOUT_FILENO, "error : Confirm client's arguuments!\n", strlen("error : Confirm client's arguuments!\n"));
		return -1;
	}

	/* create client socket */
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		write(STDOUT_FILENO, "error : socket() is failed!\n", strlen("error : socket() is failed!\n"));
		return -1;
	}

	/* set socket information */
	memset(&serv_addr, 0 , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	/*connect with server socket */
	if(connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		write(STDOUT_FILENO, "error : connect() err!\n", strlen("error : connect() err!\n"));
		return -1;
	}
	sprintf(client_pid, "%d", getpid());
	write(sock_fd, client_pid, strlen(client_pid));


	while(1)
	{
		memset(cmd, 0 , MAX);
		memset(CMD, 0 , MAX);
		fflush(stdin);
		fflush(stdout);
		write(STDOUT_FILENO, ">", 2);
		n = read(STDIN_FILENO, cmd, MAX); // recieve command from user
		if(!strcmp(cmd, "\n"))
		{
			printf("cmd\n");
			//strcpy(CMD, "error : Insert something!");
			//write(sock_fd, CMD, MAX);
			continue;
		}
		else
		{

			cmd[n-1] = ' '; // remove enter word
			
			/* command converting */
			if(convcomm(cmd, CMD) == -1)
				continue;


			if(write(sock_fd, CMD, MAX) > 0) // send message to server
			{
				if(read(sock_fd, result, MAX) > 0) // recieve message from server 
				{
					write(STDOUT_FILENO, result, strlen(result));
				}
				else
					break;
			}
			else
				break;
		}

	}
	close(sock_fd);

	return 0;
}



int convcomm(char* cmd, char* CMD)
{

	char temp[MAX];		// temp array
	char copy[MAX];		// copy of cmd array
	char oblist[20][20];	// object_list for directory
	char comm[30];
	// tokens for cut arguments
	char *token;		// command
	char *token2;		// option / path
	char *token3;		// path	  / option
	struct stat st;		// stat struct to check path is correct
	int a;		
	int ob_count=0;		// object_counter

	strcpy(copy, cmd);	// copy cmd array

	token = strtok(cmd, " ");



	// save objects' name for commands which can recieve many arguments
	if((strcmp(token, "mkdir") ==0) || (strcmp(token, "rmdir") ==0) ||(strcmp(token, "delete") ==0))
	{ 

		while(1)
	 	{
	 		token2 = strtok(NULL, " "); // cut arguments

	 		if(token2 == NULL) break;
			
			strcpy(oblist[ob_count++], token2); 
	   	}
	}
	else
	{
		token2 = strtok(NULL, " ");
		token3 = strtok(NULL, " ");
	}


	if(!strcmp(token, "ls"))
	{
		strcpy(CMD, "NLST ");

		if(token2 != '\0') // has path or option
		{
			// insert option first, path second
		  	if(!strcmp(token2, "-a"))   
			{	
				strcat(CMD, "-a ");
	 			if(token3 != '\0')	// if has path
			  		strcat(CMD, token3);
 		  	}

			// insert option first, path second
            else if(!strcmp(token2, "-l"))   
            {   	
            	strcat(CMD, "-l ");

                if(token3 != '\0')  // if has path
					strcat(CMD, token3); 
            }
		
			// insert option first, path second and confirm that path is correct or not
            else if( (!strcmp(token2, "-al")) || (!strcmp(token2, "-la")) ) 
            {      
            	strcat(CMD, "-al ");
                
                if(token3 != '\0')  // if has path
					strcat(CMD, token3);   
		   	}

		   	else if((token3 == '\0')) // only path case
 		  	{
 		  		strcpy(temp, token2);
 		  		if(temp[0] != '/')
 		  		{
					write(STDOUT_FILENO, "error : Check second arguments\n", strlen("error : Check second arguments\n"));
 		  			write(sock_fd, "error : Check second arguments\n", strlen("error : Check second arguments\n")); return -1;
 		  		}
 		  		
 		  		strcat(CMD, token2);
 		  		
 		  	}

		   	else if((token3 != '\0') && !strcmp(token3, "-a"))   
			{	
				strcat(CMD, "-a ");
	 			strcat(CMD, token2);
 		  	}

 		  	else if((token3 != '\0') && !strcmp(token3, "-l"))   
			{	
				strcat(CMD, "-l ");
	 			strcat(CMD, token2);
 		  	}

 		  	else if((token3 != '\0') && ( (!strcmp(token3, "-al")) || (!strcmp(token3, "-la")) ))   
			{	
				strcat(CMD, "-al ");
	 			strcat(CMD, token2);
 		  	}
 		  	
 		  	else
 		  	{
 		  		write(STDOUT_FILENO, "error : Check arguments\n", strlen("error : Check arguments\n"));
 		  		write(sock_fd, "error : Check arguments\n", strlen("error : Check arguments\n")); return -1;
 		  	}
		}

	}

	else if(!strcmp(token, "dir")) // if command is dir, concatnate LIST into cmd
	{
		if(token3 != NULL)
		{
			write(STDOUT_FILENO, "error : Too many argument\n", strlen("error : Too many argument\n")); ;
			write(sock_fd, "error : Too many argument\n", strlen("error : Too many argument\n")); ; return -1;
		}
		
		strcpy(CMD, "LIST ");

		if(token2 != '\0')
			strcat(CMD, token2);
	}

	else if(!strcmp(token, "pwd")) // if command is pwd, concatnate PWD into cmd
	{
		if(token2 != '\0')
		{
			write(STDOUT_FILENO, "error : Too many argument\n", strlen("error : Too many argument\n")); ;
			write(sock_fd, "error : Too many argument\n", strlen("error : Too many argument\n")); ; return -1;
		}
		strcpy(CMD, "PWD ");
	}
	
	else if(!strcmp(token, "cd")) // if command is cd
	{	
		if(token3 != NULL)
		{
			write(STDOUT_FILENO, "error : Too many argument\n", strlen("error : Too many argument\n")); ;
			write(sock_fd, "error : Too many argument\n", strlen("error : Too many argument\n")); ; return -1;
		}
		
		if(token2 == NULL) // if has no arguments, error
		{
			strcpy(CMD, "CWD ");
		}
		
		else if(!strcmp(token2,"..")) // if additional argument is '..', concatnate 'CDUP ..' into cmd array
			strcpy(CMD, "CDUP ..");
		else				// in other case, concatnate 'CWD' and argument
		{
			strcpy(CMD,"CWD ");
			strcat(CMD,token2);
		}
	}
	
	else if(!strcmp(token,"mkdir")) // if command is mkdir
	{		
		if(ob_count == 0) // if has no argument, error
		{
			write(STDOUT_FILENO, "error : Need more argument\n", strlen("error : Need more argument\n")); ;
			write(sock_fd, "error : Need more argument\n", strlen("error : Need more argument\n")); return -1;

		}

		
		else
		{
			strcpy(CMD, "MKD "); // concatnate MKD and all additional arguments
		 	for(a =0; a < ob_count ; a++)
		 	{	
				strcat(CMD, oblist[a]); strcat(CMD, " ");
		 	}
		} 
	}


	else if(!strcmp(token, "delete")) // if command is delete
	{
        if(ob_count == 0) // if has no argument, error
        {write(STDOUT_FILENO,"error : Need more argument\n", strlen("error : Need more argument\n")); ;
		 write(sock_fd,"error : Need more argument\n", strlen("error : Need more argument\n")); return -1;
		}
        
        else
        {
            strcpy(CMD, "DELE "); // concatnate DELE and additional arguments
            for(a =0; a < ob_count ; a++)
            { strcat(CMD, oblist[a]); strcat(CMD, " ");}
        }

	}

	else if(!strcmp(token, "rmdir")) // if command is rmdir
	{
        if(ob_count == 0) // if hasno argument, error
        {
        	write(STDOUT_FILENO,"error : Need more argument\n", strlen("error : Need more argument\n"));
        	write(sock_fd,"error : Need more argument\n", strlen("error : Need more argument\n")); return -1;
        }
        else
        { 
    		// concatnate RMD and additional argument
            strcpy(CMD, "RMD "); 
            for(a =0; a < ob_count ; a++)
            {strcat(CMD, oblist[a]); strcat(CMD, " ");}
        }

	}
	
	else if(!strcmp(token, "rename")) // if command is rename
	{
		if((token2 == NULL) || (token3 == NULL)) // if input doesn't have two argument, error
		{
			write(STDOUT_FILENO, "error : Need more argument\n", strlen("error : Need more argument\n"));
			write(sock_fd, "error : Need more argument\n", strlen("error : Need more argument\n"));return -1;
		}	
			// concatnate RNFR arg1 RNTO arg2
		strcpy(CMD, "RNFR ");
		strcat(CMD, token2);
		strcat(CMD, " RNTO ");
		strcat(CMD, token3);		
	}

	else if(!strcmp(token, "quit")) // if command is quit, copy QUIT into cmd array
	{
 	 	strcpy(CMD, "QUIT ");
	}

	else
	{
		write(STDOUT_FILENO, "error : Unknown Command...\n", strlen("error : Unknown Command...\n"));
		write(sock_fd, "error : Unknown Command...\n", strlen("error : Unknown Command...\n")); return -1;
	}	


	return 0;
}
void sh_term(int signal)
{
	exit(0);
}

void sh_int(int signal)
{
	write(sock_fd, "QUIT ", strlen("QUIT "));
	write(STDOUT_FILENO, "	Process quit.... \n", strlen("	Process quit.... \n"));
	exit(0);
}