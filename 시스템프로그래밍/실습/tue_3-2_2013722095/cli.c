//////////////////////////////////////////////////////////////////////////
// File Name : srv.c													//
// Date: 2017.05.30														//
// Os: Ubuntu 12.04 LTS 64bits											//
// Author: CHoi Jae eun													//
// Student ID: 2013722095 												//
// -----------------------------------------------------------------	//
// Title: System Programming Practice #3-2 (ftp server) 				//
// Description : Client part code										//
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

#include <time.h>

#define MAX 1024
char* convert_addr_to_str(unsigned long, unsigned int);
void main(int argc, char **argv)
{
	char *hostport;
	char *token1;
	char command[MAX];
	char result[MAX];
	char ip[4][20];
	char note[MAX];
	char client_ip[30];
	struct sockaddr_in srvaddr, temp;
	int sockfd, srv_data, sockfd_data, n;
	int random_port, head, tail;
	int len, result_byte = 0;


	if(argc != 3)
	{
		printf("error : check arguments!\n"); exit(1);
	}


	// make client socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// initialize server socket structure 
	memset(&srvaddr, 0 , sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = inet_addr(argv[1]);
	srvaddr.sin_port = htons(atoi(argv[2]));


	// try connecting
	if(connect(sockfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) < 0)
	{
		printf("error : Filed to connect with server!\n");
		exit(1);
	}
	// receive client's IP from connected server
	memset(client_ip,0, sizeof(client_ip));
	read(sockfd, client_ip, sizeof(note));


	write(STDOUT_FILENO, "ftp>", strlen("ftp>"));
	n = read(STDIN_FILENO, command, sizeof(command));
	command[n-1] = '\0';	// remove enter


	if(!strcmp(command, "ls"))
	{

		srand(time(NULL)); // make random value

		// get random port number(10001 ~ 30000)
		random_port = (rand()%20000)+10001;

		hostport = convert_addr_to_str(inet_addr(client_ip), random_port);
		// send PORT command to server
		sprintf(note, "PORT %s",hostport);
		write(sockfd, note, strlen(note));
		
		// receive command result from server
		read(sockfd, result, MAX);
		printf("%s", result);

		//send converted command
		write(sockfd, "NLST ", strlen("NLST"));


		/* make new link (for data) transfer */
		srv_data = socket(PF_INET, SOCK_STREAM, 0);


		memset(&temp, 0, sizeof(temp));
		temp.sin_family = AF_INET;
		temp.sin_addr.s_addr = htons(INADDR_ANY);
		temp.sin_port = htons(random_port);

		if(bind(srv_data, (struct sockaddr*)&temp, sizeof(temp)) < 0)
		{
			printf("error : Failed to bind(for data)\n");
			exit(1);
		}
		
		if(listen(srv_data, 5) < 0)
		{
			printf("error : Failed to listen(for data)\n");
			exit(1);
		}

		len = sizeof(temp);
		sockfd_data = accept(srv_data,(struct sockaddr*)&temp,&len);
		//receive signal
		memset(result, 0, sizeof(result));
		read(sockfd, result, MAX);
		write(STDOUT_FILENO, result, strlen(result));
		//receive data
		memset(result, 0, sizeof(result));
		read(sockfd_data, result, MAX);
		write(STDOUT_FILENO, result, strlen(result));
		result_byte = strlen(result)*sizeof(char);
		//receive signal
		memset(result, 0, sizeof(result));
		read(sockfd, result, MAX);
		write(STDOUT_FILENO, result, strlen(result));	

		printf("OK. %d byte is received.\n", result_byte);
	}
	else
	{
		printf("error : Unknown command!\n");
		exit(1);
	}
	exit(0);
	
}

//////////////////////////////////////////////////////////////////////////
// convert_addr_to_str													//
// ==================================================================== //
// Input: unsigned long ip_addr -> client's IP address					//
//		  unsigned int *port -> client's Port# for data transfer		//
// Output: char* addr    -> decoded IP address and Port#				//
// Purpose: decoding IP/Port# in one sentence							//
//////////////////////////////////////////////////////////////////////////

char* convert_addr_to_str(unsigned long ip_addr, unsigned int port)
{
	char *addr;
	int head,  tail;
	struct sockaddr_in temp;
	char *token, *token2, *token3, *token4;

	temp.sin_addr.s_addr = ip_addr;
	
	addr = malloc(100*sizeof(char));

	token = strtok(inet_ntoa(temp.sin_addr), ".");
	token2 = strtok(NULL, ".");
	token3 = strtok(NULL, ".");
	token4 = strtok(NULL, " ");
	
	head = port / 256;
	tail = port % 256;

	sprintf(addr, "%s,%s,%s,%s,%d,%d", token, token2, token3, token4,head,tail);
	
	return addr;
}
