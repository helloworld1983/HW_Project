///////////////////////////////////////////////////////////////////////
// File Name : cli.c 		    			   	     //
// Date: 2017.05.02						     //
// Os: Ubuntu 12.04 LTS 64bits					     //
// Author: Choi Jae eun						     //
// Student ID: 2013722095					     //
// ------------------------------------------------------------------//
// Title: System Programming Practice #2-1 	( ftp client ) 	     //
// Description : client file / send user's input to server	     //
//			and recieve its result			     // 
///////////////////////////////////////////////////////////////////////


#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define MAX_BUFF 500
#define RCV_BUFF 500


//////////////////////////////////////////////////////////////////////////
// conv_cmd								//
// ==================================================================== //
// Input: 								//
// char* buff	  -> user input command array				//
// char* cmd_buff -> converted command array				//
// Output: 0(success) / -1(fail)					//
// Purpose: convert command						//
//////////////////////////////////////////////////////////////////////////


int conv_cmd(char* buff, char* cmd_buff) // command converter
{
	if(!strcmp(buff, "ls")) // convert 'ls' to 'NLST'
		strcpy(cmd_buff, "NLST");
	else if(!strcmp(buff, "quit"))  // convert 'quit' to 'QUIT'
		strcpy(cmd_buff, "QUIT");
	else return -1; // About other commands, return -1

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// process_result                                                       //
// ==================================================================== //
// Input:                                                               //
// char* rcv_buff -> result array which recieved from server            //
// Output: none	                                                        //
// Purpose: cut rcv_buff array into token and then print it             //
//////////////////////////////////////////////////////////////////////////

void process_result(char* rcv_buff) // print recieved result
{
	write(STDOUT_FILENO, rcv_buff, strlen(rcv_buff));
}
int main (int argc, char **argv)
{
	char buff[MAX_BUFF], cmd_buff[MAX_BUFF], rcv_buff[RCV_BUFF]; // array for 'user command', 'converted command', 'recieve result'
	int n;
	char temp[100];
	int client_fd, conn_fd;						// client socket discripter / server socket discripter
	char* IP_addr;							
	char*  port_num;
	struct sockaddr_in srvaddr;					// server socket sturct

	if(argc != 3) // uncorrect input case 
	{
	write(STDERR_FILENO, "Client : Need IP address / Port Number\n", strlen("Client : Need IP address / Port Number\n")); 
	return -1;
	}


	/* open socket */
	if((client_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		write(STDERR_FILENO, "Client : socket() error!!\n", strlen("Client : socket() error!!\n"));
		return -1;
	}


	/* connect to server */
	port_num = argv[2];
	IP_addr = argv[1];

	// initialize srvaddr 
	memset(&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = PF_INET;
	srvaddr.sin_addr.s_addr = inet_addr(IP_addr);
	srvaddr.sin_port = htons(atoi(port_num));
 
	// if client failed to connect with server
	if((conn_fd = connect(client_fd,(struct sockaddr*)&srvaddr, sizeof(srvaddr))) < 0) 
	{
		write(STDERR_FILENO, "Client : connect() error!!\n", strlen("Client : connect() error!!\n"));
		return -1;
	}


	for(;;)
	{
		write(STDOUT_FILENO, "CMD>> ", strlen("CMD>> "));
		read(STDIN_FILENO, buff, MAX_BUFF);
		n = strlen(buff);
		buff[n-1] = '\0';


		if(conv_cmd(buff, cmd_buff) < 0) // convert command 
		{
			write(STDERR_FILENO, "Client : conv_cmd() error!!\n", strlen("Client : conv_cmd() error!!\n"));
			return -1;
		}

		n = strlen(cmd_buff); // get converted command's length

		// send converted command to server
		if(write(client_fd, cmd_buff, n) != n)
		{
			write(STDERR_FILENO, "Client : write() error!!\n", strlen("Client : write() error!!\n"));
			return -1;
		}
		
		// read result from server
		if((n= read(client_fd, rcv_buff, RCV_BUFF-1)) < 0)
		{
			write(STDERR_FILENO, "Client : read() error!!\n", strlen("Client : read() error!!\n"));
			return -1;
		}
		// add null into result array
		rcv_buff[n] = '\0'; 


		if(!strcmp(rcv_buff, "QUIT")) // if recieved command was quit
		{
			write(STDOUT_FILENO, "Program quit!!\n", strlen("Program quit!!\n"));
			close(client_fd);	// close socket		
			return 0;
		}
		
		process_result(rcv_buff); // display ls command result
		
	}
	return 0;	
}


