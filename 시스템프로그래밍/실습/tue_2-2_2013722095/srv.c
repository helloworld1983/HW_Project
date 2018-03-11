///////////////////////////////////////////////////////////////////////
// File Name : cli.c 		    			   	   					 //
// Date: 2017.05.08												     //
// Os: Ubuntu 12.04 LTS 64bits					     				 //
// Author: Choi Jae eun						     					 //
// Student ID: 2013722095					     					 //
// ------------------------------------------------------------------//
// Title: System Programming Practice #2-1 	( ftp client ) 	     	 //
// Description : server file / recieve client's message and	     	 //
//				 re-transfer it to client 		   					 // 	 
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <signal.h>

#define BUF_SIZE 256

void sh_chld(int); // signal handler for SIGCHLD
void sh_alrm(int); // signall handler for SIGALRM
int client_info(struct sockaddr_in*); // print client's info

int main(int argc, char **argv)
{

	char buff[BUF_SIZE];
	int n;
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;
	int len;
	int port;

	signal(SIGALRM,sh_alrm); // if alarm() function is activated, excute sh__alrm() function
	signal(SIGCHLD, sh_chld); // if child process' is changed, excute sh_chld() function

	/* create server socket */
	server_fd = socket(PF_INET, SOCK_STREAM, 0); 

	/* set server socket's information */
	memset(&server_addr, 0 , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);//htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	/* bind server socket with address */
	bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	/* prepare connection */
	listen(server_fd, 5);


	while(1)
	{
		pid_t pid;
		len = sizeof(client_addr);
		/* connect with client */
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
		

		if((pid = fork())< 0)//create child process
		{
			printf("fork() err \n");
			exit(1);
		}	

		else if(pid == 0) // child process
		{
			/* print child's process id */
			printf("Child Process ID : %d\n", getpid());

			while(1)
			{

				n = read(client_fd, buff, BUF_SIZE);	// recieve message
	
				if(!strcmp(buff, "QUIT\n")) // if message is 'QUIT'
				{
					alarm(2); // activate alarm function after 2sec
					break;		// break loop
				}

				write(client_fd, buff, n); // message re-transfer
				memset(buff, 0, BUF_SIZE); // initialize buff array
			}
		}

		else  // parent process 
		{

			client_info(&client_addr); // print child process' infomation
		}

		close(client_fd);		// close client socket
	}
		
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// sh_chld																//
// ==================================================================== //
// Input: int signum -> SIGCHLD signal									//
// Output: none															//
// Purpose: inform that child process is changed to user				//											//
//////////////////////////////////////////////////////////////////////////
void sh_chld(int signum)
{
	printf("Status of Child process was changed.\n");
	wait(NULL);
}


//////////////////////////////////////////////////////////////////////////
// sh_alrm																//
// ==================================================================== //
// Input: int signum -> SIGALRM signal									//
// Output: none															//
// Purpose: inform that child's PID and it will be teminated to user	//			//											//
//////////////////////////////////////////////////////////////////////////
void sh_alrm(int signum)
{
	printf("Child Process(PID : %d) will be terminated.\n", getpid());
	exit(1);
}



//////////////////////////////////////////////////////////////////////////
// client_info															//
// ==================================================================== //
// Input: struct sockaddr_in* clientaddr -> connected client server's	//
//						socket info										//
// Output: 0(success)  /  1(fail)										//
// Purpose: print client_info											//
//////////////////////////////////////////////////////////////////////////

int client_info(struct sockaddr_in* clientaddr)
{
	char temp[200];
	char ip[20] ;
	int port;
	if(inet_ntoa(clientaddr->sin_addr) < 0) return -1;
	else strcpy(ip, inet_ntoa(clientaddr->sin_addr));
	if(ntohs(clientaddr->sin_port) < 0) return -1;
	else port = ntohs(clientaddr->sin_port);

	sprintf(temp, "========Client info========\nIP address : %s\n\nPort # : %d \n===========================\n", ip, port);
	write(STDOUT_FILENO, temp, strlen(temp)); // print input info
	return 0;
	
}