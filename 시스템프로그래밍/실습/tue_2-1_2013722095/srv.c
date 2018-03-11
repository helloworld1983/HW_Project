///////////////////////////////////////////////////////////////////////
// File Name : srv.c                                                 //
// Date: 2017.05.02                                                  //
// Os: Ubuntu 12.04 LTS 64bits                                       //
// Author: Choi Jae eun                                              //
// Student ID: 2013722095                                            //
// ------------------------------------------------------------------//
// Title: System Programming Practice #2-1      ( ftp server )       //
// Description : server file / recieve client's message and run      //
//		 client's acquire. Then send result to client	     //
///////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#define MAX_BUFF 500
#define SEND_BUFF 500

//////////////////////////////////////////////////////////////////////////
// client_info								//
// ==================================================================== //
// Input: struct sockaddr_in* clientaddr -> connected client server's	//
//						socket info		//
// Output: 0(success)  /  1(fail)					//
// Purpose: print client_info						//
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

//////////////////////////////////////////////////////////////////////////
// cmd_process								//
// ==================================================================== //
// Input: struct sockaddr_in* clientaddr -> connected client server's	//
//						socket info		//
// Purpose: print client_info						//
//////////////////////////////////////////////////////////////////////////

int cmd_process(char* buff, char* result_buff)  // excute inserted command
{
	DIR* dp;				// for open directory
	struct dirent *dirp;
	
	if((dp = opendir(".")) < 0) // open current directory
		return -1;
	write(STDOUT_FILENO, buff, strlen(buff)); // print passed command
	write(STDOUT_FILENO, "\n", strlen("\n"));
	if(!strcmp(buff, "NLST")) // if input command is NLST
	{
		while(dirp = readdir(dp))
		{
			// print files in the current directory
			strcat(result_buff, dirp->d_name);
			strcat(result_buff, "\n");		
		}
	}
	else if(!strcmp(buff, "QUIT")) // if input command is QUIT
	{
		strcpy(result_buff,"QUIT");
	}
	else							// About other command, return -1
		return -1;

	return 0;
}


int main(int argc, char **argv)
{
	char buff[MAX_BUFF], result_buff[SEND_BUFF]; // array for 'converted command(input)', 'result (output)'
	int n;				
	int server_fd, conn_fd;						 // socket discriptor for server and client
	int clilen;									// client struct's size
	struct sockaddr_in srvaddr, cliaddr;		// socket address struct
	

	if(argc != 2)  // if wrong input case
	{
		write(STDOUT_FILENO, "Server error : Check Port Numver\n", strlen("Server error : Check Port Numver\n"));
		return -1;
	}

	/* open socket */
	if((server_fd = socket(PF_INET, SOCK_STREAM,0)) < 0)
	{	
		write(STDERR_FILENO, "Server : socket() err!!\n", strlen("Server : socket() err!!\n"));
		return -1;
	}		
	
	/* initialize server socket struct */
	memset(&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family= PF_INET;
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srvaddr.sin_port = htons(atoi(argv[1]));
	
	//bind socket with address
	if(bind(server_fd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) < 0)
	{
		write(STDERR_FILENO, "Server : bind() err!!\n", strlen("Server : bind() err!!\n"));
		return -1;//exit(1);
	}



	// listen	(5 time)
	if(listen(server_fd, 5) < 0)
	{
		write(STDERR_FILENO, "Server : listen() err!!\n", strlen("Server : listen() err!!\n"));
		return -1;

	}



	for(;;)
	{

		clilen = sizeof(cliaddr);
		/* accept client's connection */
		conn_fd = accept(server_fd, (struct sockaddr* ) &cliaddr, &clilen);
		if(conn_fd < 0)  // connection falied case
		{
			write(STDERR_FILENO, "Server : accept() err!!\n", strlen("Server : accept() err!!\n"));
			return -1;
		}
		/*display client ip and port*/
		if(client_info(&cliaddr) < 0)
		{
			write(STDERR_FILENO,"Server : client_info() err!!\n", strlen("Server : client_info() err!!\n"));
			return -1;
		}

		while(1)
		{
			n = read(conn_fd, buff, MAX_BUFF);
			buff[n] = '\0';
			
			if(cmd_process(buff, result_buff) < 0)
			/*command execute and result*/
			{
				write(STDERR_FILENO, "Server : cmd_process() err!!\n", strlen("Server : cmd_process() err!!\n"));
				break;
			}
			/* send result to client */
			write(conn_fd, result_buff, strlen(result_buff));
			
			/* if converted command is QUIT*/
			if(!strcmp(result_buff, "QUIT"))
			{
				write(STDOUT_FILENO, "Server Quit!!\n", strlen("Server Quit!!\n"));
				break;
			}
		}
		if(!strcmp(result_buff, "QUIT")) // break loop
			break;

	}
	close(server_fd); // close server socket
	return 0;
}

