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
#include <time.h>

#include <fcntl.h>

#define MAX 4096
#define FLAGS		(O_RDWR | O_CREAT | O_TRUNC)
#define	ASCII_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define BIN_MODE 	(S_IXUSR | S_IXGRP | S_IXOTH)

int sock_fd;
int asc_bin = 0;
int convcomm(char* ,char* );
void sh_int(int); 
void sh_term(int);
void log_in(int);
void port_func(char *, int);
char* convert_addr_to_str(unsigned long, unsigned int);
char *remove_rn(char *);

	

int main (int argc, char **argv) // recieve addtional argument
{
	// main variable
	char cmd[MAX];		// input from user
	char CMD[MAX]; 		// converted command
	char result[MAX];	// result from server
	char temp[MAX];
	char temp2[MAX];
	char client_pid[20];
	char client_ip[30];
	char *token, *token2;
	int n, data_len;
	int PORT_flag = 0;
	int received_data_size;


	// socket variable
	struct sockaddr_in serv_addr;

	// data socket variable
	struct sockaddr_in from_server_data_addr;
	int from_server_data_fd, client_data_fd;
	int random_port;
	int source_file_fd;
	int put_get =0;

	signal(SIGINT, sh_int); // SIGINT handler
	signal(SIGTERM, sh_term);

	/* confirm arguments */
	if(argc != 3)
	{
		write(STDOUT_FILENO, "error : Confirm client's arguments!\n", strlen("error : Confirm client's arguuments!\n"));
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
	write(sock_fd, client_pid, strlen(client_pid)); // send client pid
	read(sock_fd, client_ip, sizeof(client_ip));		// receive client ip


	log_in(sock_fd); 	// lostrcat(result, " ");g in the conneted server with ID/Password

	read(sock_fd, temp, MAX);
	write(STDOUT_FILENO, temp, strlen(temp));

	while(1)
	{

		memset(cmd, 0 , MAX);
		memset(CMD, 0 , MAX);
		memset(temp, 0, MAX);
		PORT_flag = 0;
		write(STDOUT_FILENO, ">", 2);
		n = read(STDIN_FILENO, cmd, MAX); // recieve command from user
		
		if(!strcmp(cmd, "\n"))
			continue;
		
		else
		{

			cmd[n-1] = ' '; // remove enter word
			
			srand(time(NULL)); // make random value
			random_port = (rand()%20000)+10001;// get random port number(10001 ~ 30000)
			
			strcpy(temp, cmd);
			token = strtok(temp, " ");

			//if command was ls or put or get(need data transfer port)
			if((!strcmp(token, "ls")) || (!strcmp(token, "put"))|| (!strcmp(token, "get")))
			{
				if(!strcmp(token, "ls"))	put_get = 0;
				if(!strcmp(token, "put"))	put_get = 1;
				if(!strcmp(token, "get"))	put_get = 2;

				from_server_data_fd  = socket(PF_INET, SOCK_STREAM, 0);

				memset(&from_server_data_addr, 0, sizeof(from_server_data_addr));
				from_server_data_addr.sin_family = AF_INET;
				from_server_data_addr.sin_addr.s_addr = htons(INADDR_ANY);
				from_server_data_addr.sin_port = htons(random_port);
	

				if(bind(from_server_data_fd, (struct sockaddr*)&from_server_data_addr, sizeof(from_server_data_addr)) < 0)
				{
					printf("error : Failed to bind(for data)\n");
					exit(1);
				}
				if(listen(from_server_data_fd, 5) < 0)
				{
					printf("error : Failed to listen(for data)\n");
					exit(1);
				}

				port_func(client_ip, random_port);


				read(sock_fd, temp, MAX); // receive 200


				write(STDOUT_FILENO, temp, strlen(temp));
				PORT_flag = 1;				
			}
			else
			{
				write(sock_fd, " \n", strlen(" \n"));
				read(sock_fd, temp, MAX);
			}

			/* command converting */
			if(convcomm(cmd, CMD) == -1)
				continue;

			if(write(sock_fd, CMD, MAX) > 0) // send message to server
			{
				/* get file name to send / receive */
				token2 = strtok(CMD, " ");
				token2 = strtok(NULL, " ");

				if(PORT_flag == 1)
				{
					data_len = sizeof(from_server_data_addr);
					client_data_fd = accept(from_server_data_fd, (struct sockaddr *)&from_server_data_addr, &data_len);
				}

				memset(result, 0, MAX);
				if(read(sock_fd, result, MAX) > 0) // recieve message from server 
				{

					write(STDOUT_FILENO, result, strlen(result));
					token = strtok(result, " ");

					if(!strcmp(token,"221"))
						break;
					if(!strcmp(token, "150")) // ready to receive data
					{
						received_data_size = 0;
						if(put_get == 0)	// about ls result
						{
							read(client_data_fd, result, MAX);
							write(STDOUT_FILENO, result, strlen(result));
							received_data_size = (strlen(result) * sizeof(char));

							memset(result, 0 , MAX);
							read(sock_fd, result, MAX);
							write(STDOUT_FILENO, result, strlen(result));

							memset(temp, 0, MAX);
							sprintf(temp,"%d bytes is received\n", received_data_size);
							write(STDOUT_FILENO, temp, strlen(temp));
							close(client_data_fd);
						}
						if(put_get == 1) // about put 
						{
							received_data_size = 0;	// sent data size

							source_file_fd = open(token2, O_RDONLY, 0644);

							while(1)
							{
								memset(temp2, 0 , MAX);
								memset(temp, 0, MAX);
								n = read(source_file_fd, temp2, MAX);

								if(asc_bin == 0) // binary mode
									strcpy(temp, temp2);
								else			  // ascii mode
									strcpy(temp, remove_rn(temp2));

								received_data_size += (strlen(temp)*sizeof(char)); // adding sent data size 
								
								if(strlen(temp)== 0) // EOF
								{
									write(client_data_fd, temp, MAX);
									break;
								}

								write(client_data_fd, temp, strlen(temp));	//data sending

							}
							close(source_file_fd);

							

							memset(result, 0 , MAX);
							read(sock_fd, result, MAX); // 226

							write(STDOUT_FILENO, result, strlen(result)); //226

							memset(result, 0 , MAX);
							sprintf(result, "OK. %d bytes is sent.\n", received_data_size);
							write(STDOUT_FILENO, result, strlen(result));
							close(client_data_fd);
						}

						if(put_get == 2) // about get 
						{
							// make new file
							if(asc_bin == 0)
								source_file_fd = open(token2, FLAGS, BIN_MODE);
							if(asc_bin == 1)
								source_file_fd = open(token2, FLAGS, ASCII_MODE);

							while(1)
							{
								memset(temp,0, MAX);
								read(client_data_fd, temp, MAX);	// received data

								if(strlen(temp) == 0) //finished transfer
									break;
								received_data_size += (strlen(temp)*sizeof(char));
								write(source_file_fd, temp, strlen(temp));	// recode data into new file
								

							}
							close(source_file_fd);

							memset(result, 0 , MAX);
							read(sock_fd, result, MAX); // 226

							write(STDOUT_FILENO, result, strlen(result)); //226

							memset(result, 0 , MAX);
							sprintf(result, "OK. %d bytes is received.\n", received_data_size);
							write(STDOUT_FILENO, result, strlen(result));
							close(client_data_fd);

						}
					}
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




//////////////////////////////////////////////////////////////////////////
// convcomm																//
// ==================================================================== //
// Input: char *cmd -> user input commands								//
// 			char *CMD -> converted commands   							//
// Output: none															//
// Purpose: convert user input command to FTP commands 					//										//
//////////////////////////////////////////////////////////////////////////
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


		sprintf(CMD, "RNFR %s ",token2);
		write(sock_fd, CMD, MAX);


		memset(CMD, 0, sizeof(CMD));
		read(sock_fd, CMD, MAX);
		write(STDOUT_FILENO, CMD, strlen(CMD));

		token2 = strtok(CMD, " ");
		if(!strcmp(token2, "550"))
			return -1;

		memset(CMD, 0, strlen(CMD));
		strcpy(CMD, "RNTO ");
		strcat(CMD, token3);		
	}

	else if(!strcmp(token, "quit")) // if command is quit, copy QUIT into cmd array
	{
 	 	strcpy(CMD, "QUIT ");
	}

	else if(!strcmp(token,"put"))
	{
		if(token2 == NULL)
		{
			write(STDOUT_FILENO, "error : Need more argument\n", strlen("error : Need more argument\n"));
			write(sock_fd, "error : Need more argument\n", strlen("error : Need more argument\n"));return -1;
		}			
		strcpy(CMD, "STOR ");
		strcat(CMD,token2);
	}
	else if(!strcmp(token, "get"))
	{
		if(token2 == NULL)
		{
			write(STDOUT_FILENO, "error : Need more argument\n", strlen("error : Need more argument\n"));
			write(sock_fd, "error : Need more argument\n", strlen("error : Need more argument\n"));return -1;
		}			
		strcpy(CMD, "RETR ");
		strcat(CMD,token2);
	}
	else if(!strcmp(token, "bin"))
	{
		strcpy(CMD, "TYPE I");
		asc_bin = 0;
	}
	else if(!strcmp(token, "ascii"))
	{
		strcpy(CMD, "TYPE A");
		asc_bin = 1;
	}
	else if(!strcmp(token, "type"))
	{
		if(token2 == NULL)
		{
			write(STDOUT_FILENO, "error : Need more argument\n", strlen("error : Need more argument\n"));
			write(sock_fd, "error : Need more argument\n", strlen("error : Need more argument\n"));return -1;
		}

		if(!strcmp(token2, "binary"))
		{
			strcpy(CMD, "TYPE I"); 
			asc_bin = 0;
		}
		else if(!strcmp(token2, "ascii"))
		{
			strcpy(CMD, "TYPE A");
			asc_bin = 1;
		}	
		else
		{
			write(STDOUT_FILENO, "error : Wrong argument\n", strlen("error : Wrong argument\n"));
			write(sock_fd, "error : Wrong argument\n", strlen("error : Wrong argument\n"));return -1;
		}
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



//////////////////////////////////////////////////////////////////////////
// sh_int																//
// ==================================================================== //
// Input: int signal -> SIGINT											//
// Output: none															//
// Purpose: quit current Process										//
//////////////////////////////////////////////////////////////////////////
void sh_int(int signal)
{
	write(sock_fd, "QUIT ", strlen("QUIT "));
	write(STDOUT_FILENO, "	Process quit.... \n", strlen("	Process quit.... \n"));
	exit(0);
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
	char temp[MAX];
	char user[MAX], *passwd, buf[MAX];
	char *token;

	for(;;)
	{

		n = read(sockfd, buf, MAX);		// recieve IP checking result
		buf[n] = '\0';
		token = strtok(buf, " ");

		if(!strcmp(token, "220"))		// permited IP
		{
			printf("** Client connected to Server **\n");

HERE:	// for re-type ID/PASSWD about 3time
			memset(user, 0, MAX);  // buffer initialize
			memset(temp, 0, MAX);

			/* insert ID/PASSWD from user */
			write(STDOUT_FILENO, "Input ID :", strlen("Input ID :"));
			n = read(STDIN_FILENO, user, MAX);
			user[n-1] = ' '; // remove enter

			/* send user id to server and receive its result*/
			sprintf(temp, "USER %s", user);
			write(sockfd, temp, MAX);

			memset(temp, 0, MAX);
			read(sockfd, temp, MAX);
			write(STDOUT_FILENO, temp, strlen(temp));

			token = strtok(temp, " ");

			// failed to log-in( wrong 3 times)
			if(!strcmp(token, "530"))
			{
				close(sockfd);
				exit(1);
			}

			// ID was wrong
			else if(!strcmp(token, "430"))
				goto HERE;

			else
			{
				/* send passwd to server and receive its result */
				passwd = getpass("Input passwd : ");
				sprintf(temp, "PASS %s",passwd);
				write(sockfd, temp, MAX);

				memset(temp, 0, MAX);
				read(sockfd, temp, MAX);
				write(STDOUT_FILENO, temp, strlen(temp));

				token = strtok(temp, " ");
				// PASSWD was wrong
				if(!strcmp(token, "430"))
					goto HERE;

				// failed to log-in( wrong 3 times)
				if(!strcmp(token, "530"))
				{
					close(sockfd);
					exit(1);
				}
				// matched ID/PASSWD
				else
					break;
			}

		}
		// non-permited IP
		else if(!strcmp(token, "431"))
		{
			close(sockfd);
			exit(1);
		}
	}
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


//////////////////////////////////////////////////////////////////////////
// port_func															//
// ==================================================================== //
// Input: char *client_ip -> current client's IP address				//
//			int random_port -> random port # for data transfer  		//
// Output: none															//
// Purpose: convert currents IP and random_port# to PORT message and 	//
//				send it to server										//
//////////////////////////////////////////////////////////////////////////
void port_func(char *client_ip, int random_port)
{
	char* hostport;
	char temp[MAX];

	hostport = convert_addr_to_str(inet_addr(client_ip), random_port);

	sprintf(temp, "PORT %s", hostport);
	write(sock_fd, temp, MAX);
}
//////////////////////////////////////////////////////////////////////////
// remove_rn															//
// ==================================================================== //
// Input: char *str -> data which send to sever 						//
// Output: char *addr 													//
// Purpose: send string which removed \r\n								//
//////////////////////////////////////////////////////////////////////////
char* remove_rn(char *str)
{
	char *addr;
	int i =0;
	int a =0;
	addr = malloc(MAX*sizeof(char));
	memset(addr, 0, MAX);
	while(1)
	{
		if(((str[i] == 0x0D) && (str[i+1] == 0x0A)) || ((str[i] == 0x0A) && (str[i+1] == 0x0D))) // found \r\n or \n\r
		{
			addr[a] = '\n'; // convert it into \n
			a++;
			i +=2;
		}
		else		// about else data , just copy to addr
		{
			addr[a] = str[i];
			i++;
			a++;
		}
		if(i > strlen(str)) break;
	}

	return addr;
}