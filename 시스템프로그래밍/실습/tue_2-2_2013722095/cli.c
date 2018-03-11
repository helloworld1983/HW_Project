///////////////////////////////////////////////////////////////////////
// File Name : cli.c 		    			   					     //
// Date: 2017.05.08												     //
// Os: Ubuntu 12.04 LTS 64bits									     //
// Author: Choi Jae eun											     //
// Student ID: 2013722095					 						 //
// ------------------------------------------------------------------//
// Title: System Programming Practice #2-2 	( ftp client ) 	     	 //
// Description : client file / send user's input to server	     	 //
//			and recieve it again 								     // 
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#define BUF_SIZE 256

int main(int argc, char **argv)
{
	char buff[BUF_SIZE];
	int n;
	int sockfd;
	struct sockaddr_in serv_addr;

	/* creat client socket*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* set socket information */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	/* connect with server socket */
	connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)); // connect with server

	while(1)
	{
		memset(buff, 0, BUF_SIZE); // initialize buff array
		write(STDOUT_FILENO, ">", 2);
		read(STDIN_FILENO, buff, BUF_SIZE); // recieve sentence from user

		if(write(sockfd, buff, BUF_SIZE) > 0)	// pass buff to server
		{
			if(read(sockfd, buff, BUF_SIZE) > 0)	// recieve sentence from server
				printf("from server : %s", buff);
			else
				break;
		}
		else
			break;
	}

	close(sockfd); // close client socket
	return 0;
}
