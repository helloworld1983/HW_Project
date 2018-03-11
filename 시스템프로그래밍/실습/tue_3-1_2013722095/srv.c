//////////////////////////////////////////////////////////////////////////
// File Name : srv.c													//
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
#include <pwd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <arpa/inet.h>
#include <netinet/in.h>


#define MAX_BUF 20

int log_auth(int);
int user_match(char*, char*);
int ip_check(struct sockaddr_in*);
int main(int argc, char *argv[])
{
	int listenfd, connfd;
	int ip_check_flag, len;
	struct sockaddr_in servaddr, cliaddr;
	FILE *fp_checkIP;

	if(argc != 2)
	{ printf("error : confrim argument\n"); return -1;}

	/* create socket */
	if((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		write(STDOUT_FILENO, "error : socket() is failed!\n", strlen("error : socket() is failed!\n"));
		return -1;
	}

	/* set server socket's information */
	memset(&servaddr, 0 , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	/* bind server socket with address */
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		write(STDOUT_FILENO, "error : bind() is failed!\n", strlen("error : bind() is failed!\n"));
		return -1;
	}

	/* prepare connection */
	if(listen(listenfd, 5) < 0)
	{
		write(STDOUT_FILENO, "error : listen() is failed!\n", strlen("error : listen() is failed!\n"));
		return -1;
	}


	for(;;)
	{
		len = sizeof(cliaddr);
		/*connect with client*/
		connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);	// connect server with client
		
		printf("** Client is trying to connect **\n");

		/* check 'client's IP can access '*/
		ip_check_flag = ip_check(&cliaddr);	


		if(ip_check_flag == 1)	// permited IP
		{
			printf(" - IP   : %s\n", inet_ntoa(cliaddr.sin_addr));
			printf(" - PORT : %d\n", ntohs(cliaddr.sin_port));
			write(connfd, "ACCEPTED", strlen("ACCEPTED")); // send ACCEPTED to client
		
		}
	
		else
		{
			write(connfd, "REJECTION", strlen("REJECTION"));	// send REJECTION to client
			printf("** It is NOT authenticated client **\n");
			close(connfd);
			continue;  // or return 0;
		}

		if(log_auth(connfd) == 0)
		{
			printf("** Fail to log-in **\n");
			close(connfd);
			continue;
		}

		/* success to log-in */
		write(connfd, "OK", strlen("OK"));
		close(connfd);
	}

}



//////////////////////////////////////////////////////////////////////////////
// log_auth																	//
// ======================================================================== //
// Input: int connfd -> client socket discriptor							//
// Output: 1(permited User)	/	0(Unpermited User)							//
// Purpose: recieve User ID/Passwd from client and then		 				//
//			confirm who want to connect with server is permited User or not	//
//////////////////////////////////////////////////////////////////////////////

int log_auth(int connfd)
{
	char user[MAX_BUF];
	char *token1, *token2;
	int n, count = 1;

	while(1)
	{
		/* recieve username and password from client*/
		read(connfd, user, MAX_BUF);
		//printf("%s\n", user);
		token1 = strtok(user, " ");
		token2 = strtok(NULL, " ");

		printf("User is trying to log-in (%d/3)\n", count);

		/* ID/PASSWORD checking*/
		if((n = user_match(token1, token2))==1)
		{
			printf("** Success to log-in **\n");
			break;
		}

		else if(n == 0)	// failed to log-in
		{
			if(count >= 3)// failed more than 3 times, disconnect client
			{
				printf("** Log-in failed **\n");
				write(connfd, "DISCONNECTION", strlen("DISCONNECTION"));
				return 0;

			}
			//forgive it about 3 times
			write(connfd, "FAIL", strlen("FAIL"));
			printf("** Log-in failed **\n");
			count++;
			continue;
		}

	}
	return 1;

}

//////////////////////////////////////////////////////////////////////////
// user_match															//
// ==================================================================== //
// Input: char *user -> recieved user Id 								//
//		  char *passwd -> recieved user passwd							//
// Output: 0(Unpermited User)	/	1(permited User)					//
// Purpose: With recieved ID and Password, confirm they are consist in  //
//			the access.txt file or not 									//
//////////////////////////////////////////////////////////////////////////

int user_match(char *user, char *passwd)
{
	FILE* fp;
	int flag = 0;
	struct passwd *pw;
	fp = fopen("passwd", "r");

	while(!feof(fp))
	{
		pw = fgetpwent(fp);

		if(!strcmp(user, pw->pw_name) && !strcmp(passwd, pw->pw_passwd))
		{
			flag=1;
			break;
		}
		else
			continue;
	}

	fclose(fp);
	if(flag == 0)
		return 0;
	else
		return 1;
}

//////////////////////////////////////////////////////////////////////////
// ip_check																//
// ==================================================================== //
// Input: struct sockaddr_in* cliaddr -> client who want to connect		//	 
// Output: 0(Unpermited IP)	/	1(permited ip)							//
// Purpose: confirm client who trying to connect is permited IP or NOT  //
//////////////////////////////////////////////////////////////////////////


int ip_check(struct sockaddr_in* cliaddr)
{
	FILE* fp;
	int flag = 0;
	char ip[50];
	char access[50];
	char temp[50];
	char *token1, *token2, *token3, *token4;
	char compare[4][10];
	strcpy(ip, inet_ntoa(cliaddr->sin_addr));

	/*get client's Dotted IP address*/
	sprintf(temp, "%s", strtok(ip, "."));
	strcpy(compare[0], temp);
	sprintf(temp, "%s", strtok(NULL, "."));
	strcpy(compare[1], temp);
	sprintf(temp, "%s", strtok(NULL, "."));
	strcpy(compare[2], temp);
	sprintf(temp, "%s", strtok(NULL, " "));
	strcpy(compare[3], temp);


	fp = fopen("access.txt", "r");

	while(!feof(fp))
	{

		fgets(access, sizeof(access), fp); // get line and seperate it 

		token1 = strtok(access, ".");
		token2 = strtok(NULL, ".");
		token3 = strtok(NULL, ".");
		token4 = strtok(NULL, " ");

		if(strcmp(token1, "*") != 0)	// if first 8bit isn't a wild card
		{
			if(strcmp(token1, compare[0]) != 0) // not matched first 8bit
				continue;
		}
		
		if(strcmp(token2, "*") != 0)	// if second 8bit isn't a wild card
		{
			if(strcmp(token2, compare[1]) != 0) // not matched second 8bit
				continue;
		}

		if(strcmp(token3, "*") != 0)	// if third 8bit isn't a wild card
		{
			if(strcmp(token3, compare[2]) != 0) // not matched first 8bit
				continue;

		}

		if(strcmp(token4, "*") != 0)	// if forth 8bit isn't a wild card
		{
			if(strcmp(token4, compare[3]) != 0) // not matched first 8bit
				continue;
		}

		flag = 1;
		break;
	}
	fclose(fp);

	if(flag == 1)
		return 1;

	else 
		return 0;

}