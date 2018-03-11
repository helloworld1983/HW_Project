//////////////////////////////////////////////////////////////////////////
// File Name : srv.c													//
// Date: 2017.05.30														//
// Os: Ubuntu 12.04 LTS 64bits											//
// Author: CHoi Jae eun													//
// Student ID: 2013722095 												//
// -----------------------------------------------------------------	//
// Title: System Programming Practice #3-2 (ftp server) 				//
// Description : server part code										//
//////////////////////////////////////////////////////////////////////////


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX 1024
void sort_func(char(*)[50], int);
char* convert_str_to_addr(char * , unsigned int *);
void main(int argc, char **argv)
{
	char *host_ip;
	char *token1, *token2, *token3, *token4;
	char temp[MAX];
	char ip[50];
	char port[50];
	char result[MAX];
	char arr[30];
	char oblist[50][50];
	int ob_count = 0;
	int decoded_port = 0;
	unsigned int port_num;

	DIR *dp;
	struct dirent *dirp;

	int listenfd, connfd, connfd_data;
	int len, a = 0;
	struct sockaddr_in servaddr, cliaddr, cliaddr_data;

	if(argc != 2)
	{ printf("error : confrim argument\n"); exit(1);}

	/* create socket(for signal, command) */
	if((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		write(STDOUT_FILENO, "error : socket() is failed!\n", strlen("error : socket() is failed!\n"));
		exit(1);
	}

	/* set server socket's information */
	memset(&servaddr, 0 , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	/* bind server socket(for signal, command) with address */
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		write(STDOUT_FILENO, "error : bind() is failed!\n", strlen("error : bind() is failed!\n"));
		exit(1);
	}

	/* prepare connection */
	if(listen(listenfd, 5) < 0)
	{
		write(STDOUT_FILENO, "error : listen() is failed!\n", strlen("error : listen() is failed!\n"));
		exit(1);	
	}

	
	memset(ip, 0 , sizeof(ip));
	memset(port, 0, sizeof(port));
	memset(temp, 0, sizeof(temp));
	memset(result, 0, sizeof(result));

	len = sizeof(cliaddr);
	/*connect with client*/
	connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);	// connect server with client (for signal, command)

	strcpy(result, inet_ntoa(cliaddr.sin_addr));
	write(connfd, result, strlen(result)); // pass client's IP address
	memset(result, 0, sizeof(result));
	
	if(connfd == 0)
	{printf("error : failed to connect with client.\n"); exit(1);}

	read(connfd, temp, MAX);
	printf("%s\n", temp);

	write(STDOUT_FILENO, "200 Port command successful\n", strlen("200 Port command successful\n"));
	write(connfd, "200 Port command successful\n", strlen("200 Port command successful\n"));

	/* decode message from client*/
	host_ip = convert_str_to_addr(temp, (unsigned int*) &port_num);

	memset(temp, 0, sizeof(temp));
	read(connfd, temp, MAX);
	write(STDOUT_FILENO, temp, strlen(temp));
	write(STDOUT_FILENO, "\n", strlen("\n"));

	if(strcmp(temp, "NLST") != 0)
	{
		close(connfd);
		exit(1);	
	}

	/* make socket for data transfer */
	connfd_data = socket(AF_INET, SOCK_STREAM, 0);
	memset(&cliaddr_data, 0, sizeof(cliaddr_data));
	cliaddr_data.sin_family = AF_INET;
	cliaddr_data.sin_addr.s_addr = inet_addr(host_ip);
	cliaddr_data.sin_port = htons(port_num);

	/* connect with client for data transfer with appointed port */
	if(connect(connfd_data, (struct sockaddr*)&cliaddr_data, sizeof(cliaddr_data)) < 0)
	{	
		printf("error : Failed to connect with client(for data)\n");
		exit(0);
	}

	write(STDOUT_FILENO, "150 Opening data connection for directory list\n", strlen("150 Opening data connection for directory list\n"));
	write(connfd, "150 Opening data connection for directory list\n", strlen("150 Opening data connection for directory list\n"));
	sleep(1); // making delay transfer

	/* NLST command excution */
	dp = opendir(".");
	while(dirp = readdir(dp))
	{
		strcpy(arr, dirp->d_name);
		if(arr[0] == '.') 
			continue;
		strcpy(oblist[ob_count], dirp->d_name);
		ob_count++;
	}

	/*sort result */
	sort_func(oblist, ob_count);

	/*concatnate result into array*/
	for(a = 0; a < ob_count; a++)
	{
		strcat(result, oblist[a]);
		strcat(result, "\n");
	}
	// send NLST result
	write(connfd_data, result, strlen(result));

	// send 226
	write(STDOUT_FILENO, "226 Result is sent successfully.\n", strlen("226 Result is sent successfully.\n"));
	write(connfd, "226 Result is sent successfully.\n", strlen("226 Result is sent successfully.\n"));
}


//////////////////////////////////////////////////////////////////////////
// convert_str_to_addr													//
// ==================================================================== //
// Input: char* str 	-> string received from client(signal sentence)	//
//		  unsigned int *port -> will receive decoded Port #				//
// Output: char* addr    -> decoded IP address of client				//
// Purpose: separate IP/Port# and decode them							//
//////////////////////////////////////////////////////////////////////////

char* convert_str_to_addr(char *str, unsigned int *port)
{
	char *addr;
	int *decoded_port;
	int head,  tail;
	char *token, *token2, *token3, *token4;
	
	addr = malloc(100*sizeof(char));


	token = strtok(str, " "); // remove 'PORT' word
	// seperate ip address
	token = strtok(NULL, ",");
	token2 = strtok(NULL, ",");
	token3 = strtok(NULL, ",");
	token4 = strtok(NULL, ",");

	sprintf(addr, "%s.%s.%s.%s", token, token2, token3, token4);

	// decode port number
	token3 = strtok(NULL, ",");
	token4 = strtok(NULL, " ");
	
	head = atoi(token3) * 256;
	tail = atoi(token4);

	*port = head + tail;

	return addr;
}

//////////////////////////////////////////////////////////////////////////
// sort_func															//
// ==================================================================== //
// Input: char** oblist -> object list in the directory					//
//			int  ob_count -> object's count								//
// Output: none															//
// Purpose: bubble sort	 												//
//////////////////////////////////////////////////////////////////////////

void sort_func(char oblist[50][50], int ob_count)
{
	char com1[50];
	char com2[50];
	char temp[50];
	int a, b, c;


	for(a = 0; a < ob_count; a++) // bubble sort
    {
        for(b=0; b < ob_count-1; b++)
        {
     	    strcpy(com1, oblist[b]);
            strcpy(com2, oblist[b+1]);

                // capitalizing name to compare
            for(c = 0; c<strlen(com1); c++)
            {if((com1[c]>=65) && (com1[c]<=90)) com1[c] += 32;}
            for(c =0; c< strlen(com2); c++)
            {if((com2[c] >= 65)&&(com2[c] <=90)) com2[c] +=32;}
                            
          	if(strcmp(com1, com2)>0) // if com1 is bigger
            {
                strcpy(temp, oblist[b]);
                strcpy(oblist[b], oblist[b+1]);
                strcpy(oblist[b+1], temp);
            }
        }
    }
}
