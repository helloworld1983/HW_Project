//////////////////////////////////////////////////////////////////////////
// File Name : cli.c													//
// Date: 2017.05.23														//
// Os: Ubuntu 12.04 LTS 64bits											//
// Author: CHoi Jae eun													//
// Student ID: 2013722095 												//
// -----------------------------------------------------------------	//
// Title: System Programming practice #3-1	 (ftp)		 				//
// Description : Restrict client's connection to server with IP,		//
//															ID/PASSWD	//
//////////////////////////////////////////////////////////////////////////


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#define MAX_BUF 20
#define CONT_PORT 20001

void log_in(int);

int main(int argc, char *argv[])
{
	int sockfd, n, p_pid;
	struct sockaddr_in servaddr;

	if(argc != 3)
	{
		printf("error : confirm arguments\n");
		return -1;
	}

	/* make client socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* initialize server socket structure */
	memset(&servaddr, 0 , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));


	if(connect(sockfd,(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("error : failed to connect with server\n");
		return -1;
	}

	log_in(sockfd);	// log in the conneted server with ID/Password
	
	close(sockfd);
	
	return 0;
}


//////////////////////////////////////////////////////////////////////////
// log_in																//
// ==================================================================== //
// Input: int sockfd -> socket discriptor								//
// Output: none															//
// Purpose: recieve IP checking result from server and then 			//
//			if permited IP, pass user ID/Password to server to get 		//
//			access permition											//
//////////////////////////////////////////////////////////////////////////
void log_in(int sockfd)
{
	int n;
	char user[MAX_BUF], *passwd, buf[MAX_BUF];

	for(;;)
	{
		n = read(sockfd, buf, MAX_BUF);		// recieve IP checking result
		buf[n] = '\0';

		if(!strcmp(buf, "ACCEPTED"))		// permited IP
		{
			printf("** It is connected to Server **\n");

HERE:	// for re-type ID/PASSWD about 3time
			memset(user, 0, sizeof(user));  // buffer initialize

			/* insert ID/PASSWD from user */
			write(STDOUT_FILENO, "Input ID :", strlen("Input ID :"));
			n = read(STDIN_FILENO, user, MAX_BUF);
			user[n-1] = ' '; // remove enter

			passwd = getpass("Input passwd : ");

			sprintf(buf, "%s%s",user, passwd);
			write(sockfd, buf, sizeof(buf));	// send it to server 

			n = read(sockfd, buf, MAX_BUF);		// recieve ID/PASSWD checking result
			buf[n] = '\0';

			if(!strcmp(buf, "OK"))				//permited ID/PASSWD
			{
				printf("** User '%s' logged in **\n", user);
				break;	
			}
			else if(!strcmp(buf, "FAIL"))		// unpermited ID/PASSWD
			{
				printf("** Log-in failed **\n");
				goto HERE;						// go to re-type ID/PASSWD
			}
			else	// DISCONNECTION
			{
				printf("** Connection closed **\n");
				break;
			}

		}
		else // REJECTION
		{
			printf("** Connection refused **\n");
			exit(0);
		}
	}
}