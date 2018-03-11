//////////////////////////////////////////////////////////////////////////
// File Name : srv.c													//
// Date: 2017.06.04													//
// Os: Ubuntu 12.04 LTS 64bits											//
// Author: CHoi Jae eun													//
// Student ID: 2013722095 												//
// -----------------------------------------------------------------	//
// Title: System Programming Assignment #3 (ftp server) 				//
// Description : server part code										//
//////////////////////////////////////////////////////////////////////////


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <time.h>

#include <signal.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <fcntl.h>

#define MAX 4096
#define FLAGS		(O_RDWR | O_CREAT | O_TRUNC)
#define	ASCII_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define BIN_MODE 	(S_IXUSR | S_IXGRP | S_IXOTH)

void sh_chld(int); // signal handler for SIGCHLD
void sh_alrm(int); // signal handler for SIGALRM
void sh_int(int); // signal handler for SIGINT
void sh_term(int);
int client_info(struct sockaddr_in*); // print client's info
void print_process();
void sort_func(char(*)[50], int);
void get_permi(char(*)[50], char*, int);
void welcome(int);
int ip_check(struct sockaddr_in*);
int user_match(char *);
int pass_match(char *, char *);
int log_auth(int, struct sockaddr_in*);

char* convert_str_to_addr(char *, unsigned int *);
char* what_time();
char* remove_rn(char *);
typedef struct Node
{
	int pid;
	int port;
	int client_pid;
	char ip[30];
	char ID[30];
	struct Node* next;
	time_t	mytime;

}Node;

typedef struct List
{
	struct Node* phead;
	struct Node* ptail;
	int child_count;
}List;

int log_fd;
List* list;
char user_id[30];

int main(int argc, char** argv)
{
	/* socket variable */
	int n;
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;
	int len;
	int port;
	char client_pid_arr[MAX];
	char client_ip_arr[30];


	/* data_socket variable */
	int n_data, data_trans_flag = 0;
	int to_client_data_fd;
	int data_size;
	unsigned int data_port;
	char *ip_arr_data;
	struct sockaddr_in to_client_data_addr;
	int file_des;
	int asc_bin = 0;


	char buffer[MAX]; // recieve system buffer here
	char result[MAX];	// command excute result
	char temp[MAX];	// temp
	char temp2[MAX];
	char oblist[50][50]; // save objects name in a directory
	char* token;	    
	char* token2;
	char* token3;
	DIR *dp;		// directory pointer
	struct dirent *dirp;	// object pointer in a directory
	struct stat st;
	struct passwd *user_pw;
	uid_t uid;
	struct tm *t;		// time struct	
	int ob_count = 0;	// object counter
	int a, b, c =0;
	int flag =0;		// flag for RNFR & RNTO
	int D_flag =0;

	/* log-in variable*/
	int ip_check_flag = 0;

	/* dynamic allocation of list*/
	list = (List*)malloc(sizeof(List));
	list->phead = NULL;
	list->ptail = NULL;
	list->child_count = 0;


	Node* cli_node;
	Node* temp_node;
	Node* temp2_node;


	/* open log file */
	log_fd = open("logfile", O_RDWR|O_CREAT|O_APPEND, 0644);


	/* create socket */
	if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		write(STDOUT_FILENO, "error : socket() is failed!\n", strlen("error : socket() is failed!\n"));
		return -1;
	}

	/* set server socket's information */
	memset(&server_addr, 0 , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	/* bind server socket with address */
	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		write(STDOUT_FILENO, "error : bind() is failed!\n", strlen("error : bind() is failed!\n"));
		return -1;
	}

	/* prepare connection */
	if(listen(server_fd, 10) < 0)
	{
		write(STDOUT_FILENO, "error : listen() is failed!\n", strlen("error : listen() is failed!\n"));
		return -1;
	}
	
	// log about starting server
	sprintf(temp, "%s Server is started.\n", what_time());
	write(log_fd, temp, strlen(temp));
	sync();
	memset(temp, 0, MAX);

	

	/* signal handler */
	signal(SIGCHLD, sh_chld);
	signal(SIGALRM, sh_alrm);
	signal(SIGINT, sh_int);
	signal(SIGTERM, sh_term);

	while(1)
	{

		pid_t pid;
		len = sizeof(client_addr);

		/* connect with client */
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);

		/* recieve client's pid */
		read(client_fd, client_pid_arr, MAX);
		/* send client's IP address */
		strcpy(client_ip_arr, inet_ntoa(client_addr.sin_addr));
		write(client_fd, client_ip_arr, sizeof(client_ip_arr));



		write(STDOUT_FILENO, "** Client is trying to connect **\n", strlen("** Client is trying to connect **\n"));

		/* check 'client's IP can access '*/
		ip_check_flag = ip_check(&client_addr);

		if(ip_check_flag == 1)	// permited IP
		{
			printf(" - IP   : %s\n", inet_ntoa(client_addr.sin_addr));
			printf(" - PORT : %d\n", ntohs(client_addr.sin_port));
			write(client_fd, "220 accessable client\n", strlen("220 accessable client\n")); 
		
		}
	
		else	// non-permited IP
		{
			write(client_fd, "431 This client can't access. Close the session\n", strlen("431 This client can't access. Close the session\n"));	// send REJECTION to client
			write(STDOUT_FILENO, "** It is NOT authenticated client **\n", strlen("** It is NOT authenticated client **\n"));
			close(client_fd);
			continue; 
		}

		/* check user id and passwd*/

		if(log_auth(client_fd, &client_addr) == 0)
		{
			write(STDOUT_FILENO,"** Fail to log-in **\n", strlen("** Fail to log-in **\n"));
			close(client_fd);
			continue;
		}

		welcome(client_fd);

		pid = fork();

		temp_node = NULL;
		temp2_node = NULL;

		/* remove node which finished process*/
	
		if(pid < 0)//create child process
		{
			printf("fork() err \n");
			continue;
		}

		else if(pid == 0) // child process
		{
			
			while(1)
			{
				/* initialize arraies*/
				memset(buffer, 0, sizeof(buffer));
				memset(temp, 0, sizeof(temp));
				memset(result, 0, sizeof(result));
				memset(oblist, 0, sizeof(oblist));
				data_trans_flag = 0;
				token = NULL;
				token2 = NULL;
				token3 = NULL;
				ob_count = 0;
				D_flag = 0;

				/* receive client's requirment about Data transfer port*/
				read(client_fd, buffer, MAX);
				write(STDOUT_FILENO, buffer, strlen(buffer));
				write(STDOUT_FILENO, "\n", strlen("\n"));;
				token = strtok(buffer, " ");



				if(!strcmp(token, "PORT"))
				{
					
					data_trans_flag = 1;
					token = strtok(NULL, " ");

					/* create socket for transport data to client*/
					if((to_client_data_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
					{
						write(STDOUT_FILENO, "error : socket()(for data) is failed!\n", strlen("error : socket()(for data) is failed!\n"));
						break;
					}

					ip_arr_data = convert_str_to_addr(token, (unsigned int*)&data_port);

					/* set socket information for data transfer */
					memset(&to_client_data_addr, 0 , sizeof(to_client_data_addr));
					to_client_data_addr.sin_family = AF_INET;
					to_client_data_addr.sin_addr.s_addr = inet_addr(ip_arr_data);
					to_client_data_addr.sin_port = htons(data_port);

					/*connect with server socket */
					if(connect(to_client_data_fd, (struct sockaddr *)&to_client_data_addr, sizeof(to_client_data_addr)) < 0)
					{
						write(client_fd, "550 Failed to access.\n", strlen("550 Failed to access.\n"));
						sprintf(temp, "%s [%s:%d] %s 550 Port command is Successful.\n", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id);
						write(log_fd, temp, strlen(temp)); sync();
						break;
					}
	
					
					write(client_fd, "200 Port command is Successful.\n", strlen("200 Port command is Successful.\n"));
					sprintf(temp, "%s [%s:%d] %s 200 Port command is Successful.\n", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id);
					write(log_fd, temp, strlen(temp)); sync();
				}
				else 
				{
					write(client_fd, " \n", strlen(" \n"));
				}

				/* recieve converted command from client*/
				n = read(client_fd, buffer, MAX);
				buffer[n-1] = ' ';
				sprintf(temp, "%s [%s:%d] %s %s\n", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id, buffer);
				write(log_fd, temp, strlen(temp));
				sync();
				memset(temp, 0, MAX);
				


				sprintf(temp,"%-s [%d]\n", buffer, getpid());
				write(STDOUT_FILENO, temp, strlen(temp));

				token = strtok(buffer, " ");

				/* if client sent error message */
				if(!strcmp(token, "error"))
					continue;


				/* save additional arguments' of under commands' */
				if((strcmp(token, "MKD") ==0) || (strcmp(token, "RMD")==0) || (strcmp(token, "DELE") ==0))
				{
					while(1) // count object's number and save its name
					{
						token2 = strtok(NULL, " ");
						if(token2 == NULL) break;
						strcpy(oblist[ob_count++], token2);
					}
				}
				else // in other command case, cut and set token like under that
				{
					token2 = strtok(NULL, " ");
					token3 = strtok(NULL, " ");
				}			


				if(!strcmp(token, "NLST"))
				{
				
					memset(temp, 0, MAX);

					write(client_fd, "150 Opening data connection for directory list.\n", strlen("150 Opening data connection for directory list.\n"));
					sprintf(temp, "%s [%s:%d] %s 150 Opening data connection for directory list.\n", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id);
					write(log_fd, temp, strlen(temp)); sync();

					/* no option, no path */
					if(token2 == '\0')
					{
						dp = opendir("."); // open current directory
			            while(dirp = readdir(dp)) // save objects' name into array except some object which start with .
			            {   
			            	strcpy(temp, dirp->d_name);
							
							if(temp[0] == '.') continue; // won't receive hidden file
							
							strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
						}

						sort_func(oblist, ob_count);

						for(a = 0; a < ob_count; a++)
						{
							if((a != 0) && (a%5 == 0))
								strcat(result, "\n");
							sprintf(temp, "%-15s  ", oblist[a]);
							strcat(result, temp);
						}

					}


					/* only path or only option */
					if((token2 != '\0') && (token3 == '\0'))
					{
						if(!strcmp(token2, "-a"))
						{
							dp = opendir("."); // open current directory
				            while(dirp = readdir(dp)) // save objects' name into array except some object which start with .
				            {   
				            	strcpy(temp, dirp->d_name);
								strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
							}

							sort_func(oblist, ob_count);

							for(a = 0; a < ob_count; a++)
							{
								if((a != 0) && (a%5 == 0))
									strcat(result, "\n");
								sprintf(temp, "%-15s  ", oblist[a]);
								strcat(result, temp);
							}

						}


						else if(!strcmp(token2, "-l"))
						{
							dp = opendir("."); // open current directory
				            while(dirp = readdir(dp)) // save objects' name into array except some object which start with .
				            {   
				            	strcpy(temp, dirp->d_name);
				            	if(temp[0] == '.') continue;
								strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
							}

							sort_func(oblist, ob_count);

							get_permi(oblist, result, ob_count);

						}
						else if(!strcmp(token2, "-al"))
						{
							dp = opendir("."); // open current directory
				            while(dirp = readdir(dp)) // save objects' name into array except some object which start with .
				            {   
				            	strcpy(temp, dirp->d_name);
								strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
							}

							sort_func(oblist, ob_count);

							get_permi(oblist, result, ob_count);
						}
						else // only path
						{
							stat(token2, &st);
							if(!(S_ISDIR(st.st_mode) | S_ISREG(st.st_mode))) // if 'path' is not a openable type
								strcpy(result, "error : wrong path!\n");
							else
							{
								if((dp = opendir(token2)) == NULL) // open current directory
								{
									strcpy(result, "error : wrong path!\n");
								}
								
								else
								{
									while(dirp = readdir(dp)) // save objects' name into array except some object which start with .
						            {   
						            	strcpy(temp, dirp->d_name);
										
										if(temp[0] == '.') continue; // won't receive hidden file
										
										strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
									}

									sort_func(oblist, ob_count);

									for(a = 0; a < ob_count; a++)
									{
										if((a != 0) && (a%5 == 0))
											strcat(result, "\n");
										sprintf(temp, "%-15s  ", oblist[a]);
										strcat(result, temp);
									}
								}
					            
							}
						}

					}

					/* both option, path*/
					if((token2 != '\0') && (token3 != '\0'))
					{
						
						if((dp = opendir(token3)) == NULL) // open directory
						{
							strcpy(result, "error : wrong path!\n");
						}
						else
						{
							if(strcmp(token2, "-l") == 0)
							{

					            while((dirp = readdir(dp))) // save objects' name into array except some object which start with .
					            {   

					            	strcpy(temp, dirp->d_name);
					            	if(temp[0] == '.') continue;
									strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
								}

								sort_func(oblist, ob_count);
								get_permi(oblist, result, ob_count);

							}
							else if(!strcmp(token2, "-a"))
							{
					            while((dirp = readdir(dp))) // save objects' name into array except some object which start with .
					            {   
					            	strcpy(temp, dirp->d_name);
									strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
								}

								sort_func(oblist, ob_count);

								for(a = 0; a < ob_count; a++)
								{
									if((a != 0) && (a%5 == 0))
										strcat(result, "\n");
									sprintf(temp, "%-15s  ", oblist[a]);
									strcat(result, temp);
								}

							}

							else if(!strcmp(token2, "-al"))
							{	
					            while((dirp = readdir(dp))) // save objects' name into array except some object which start with .
					            {   
					            	strcpy(temp, dirp->d_name);
									strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
								}

								sort_func(oblist, ob_count);

								get_permi(oblist, result, ob_count);
							}
						}
						
					}
					strcat(result, "\n");

					data_size = (strlen(result)*sizeof(char));
					write(to_client_data_fd, result, MAX);

					write(client_fd, "226 Complete transmission\n",strlen("226 Complete transmission\n"));

					memset(temp, 0, MAX);
					sprintf(temp, "%s [%s:%d] %s 226 Complete transmission | %d bytes\n", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id, data_size);
					write(log_fd, temp, strlen(temp));	sync();

					close(to_client_data_fd); // close data port to client
				}

				/* command : dir */
				if(!strcmp(token, "LIST"))
				{
					if(token2 == '\0') // no additional path
					{
						dp = opendir(".");
						while(dirp = readdir(dp)) // save objects' name into array except some object which start with .
			            {   
			            	strcpy(temp, dirp->d_name);
							strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
						}

						sort_func(oblist, ob_count); // sort

						get_permi(oblist, result, ob_count); // get file/directory's permission
					}
					else	// have additional path
					{
						
						if((dp = opendir(token2)) == NULL)
						{
							strcpy(result, "error : wrong path!\n");
						}
						else
						{
							while(dirp = readdir(dp)) // save objects' name into array except some object which start with .
				            {   
				            	strcpy(temp, dirp->d_name);
								strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
							}

							sort_func(oblist, ob_count); // sort

							get_permi(oblist, result, ob_count); // get file/directory's permission
						}						
					}
				}

				// print current working directory
				else if(!strcmp(token, "PWD"))
				{
					getcwd(temp, MAX);
					sprintf(result,"257 \"%s\" is current directory.\n",temp);
				}

				// move current working dirctory
				else if(!strcmp(token, "CWD"))
				{
					if(token2 == '\0') // no additional path
					{
						
						uid = getuid(); // get user id
						
						user_pw = getpwuid(uid);
						
						strcpy(temp, "/home/");
						strcat(temp, user_pw->pw_name); // concatenate user name
						chdir(temp);						
						strcpy(result, "250 CWD command succeeds.\n");

					}
					else if(chdir(token2) != 0) // have additional path and it was wrong 
					{	
						sprintf(result, "550 %s : Can't find such file or directory.\n",token2);
					}
					else
					{
						strcpy(result, "250 CWD command succeeds.\n");
					}
				}
				
				else if(!strcmp(token, "CDUP"))
				{
					if(chdir("..") != 0)
						strcpy(result, "550 Can't move to the parent folder.\n");
			       	else
			       		strcpy(result, "250 CWD command succeeds.\n");
				}

			    // make new directory
				else if(!strcmp(token, "MKD"))
				{
					umask(0); // set umask to zero
			 		for(a =0; a < ob_count; a++)
					{
			 			if(mkdir(oblist[a],0744) != 0) // if failed to make directory
						{
			        	   	sprintf(temp, "550 %s : Can't find such file or directory.\n", oblist[a]);
						 	strcat(result, temp);
						 	D_flag = 1;
						}
					}

					if(D_flag == 0)
			        	strcpy(result, "250 MKD command performed successfully.\n");
					
				}

				// delete object(file, directory)
				else if(!strcmp(token, "DELE"))
				{
			        for(a =0; a < ob_count; a++)
			        {
			            if(remove(oblist[a]) != 0) // if failed to make directory
			            {
			        	   	sprintf(temp, "550 %s : Can't find such file or directory.\n", oblist[a]);
			              	strcat(result, temp);
			               	D_flag = 1;
			            }			                
			        }

			        if(D_flag == 0)
			        	strcpy(result, "250 DELE command performed successfully.\n");
				}

				// remove directory
				else if(!strcmp(token, "RMD"))
				{
					for(a =0; a < ob_count; a++)
			        {
			            if(rmdir(oblist[a]) == -1) // if failed to make directory
			            {
			        	   	sprintf(temp, "550 %s : Can't find such file or directory.\n", oblist[a]);
			                strcat(result, temp);
			                D_flag = 1;
			            }
			        }

			        if(D_flag == 0)
			        	strcpy(result, "250 RMD command performed successfully.\n");
				}
				
				else if(!strcmp(token, "RNFR")) // rename:oldname
				{
					dp = opendir("."); // open current directory
					
					while(dirp = readdir(dp))
					{ 
						if(!strcmp(token2,dirp->d_name)) 
							flag = 1;
					} // if program found input named file, set flag as 1
					
					if(flag == 0) // couldn't find file
					{
						sprintf(result, "550 %s : Can't find such file or directory\n", token2);
					}
					else // found file
					{
						//  send message(find inserted file name)
						write(client_fd, "350 File exists, Ready to rename.\n", strlen("350 File exists, Ready to rename.\n"));
						
						//receive alternative name
						read(client_fd, temp, MAX);
						token3 = strtok(temp, " ");
						token3 = strtok(NULL, " ");
						printf("RNTO : %s\n", token3);
						
						if(rename(token2, token3) != 0) // if newnamed file is already exist, error
						{
							sprintf(result, "550 %s : can't be renamed.\n", token3);
						}
						else
						{
							strcpy(result, "250 RNTO command succeeds.\n");
						}

					}

				}

				// Quit
				else if(!strcmp(token, "QUIT"))
				{
					write(client_fd, "221 Goodbye....!\n", strlen("221 Goodbye....!\n"));
					exit(0); // close current child process 
				}
				// TYPE : change data transfer mode
				else if(!strcmp(token, "TYPE"))
				{

					if(!strcmp(token2,"A")) // ascii mode
					{
						asc_bin = 1;
						strcpy(result,"201 Type set to A\n");
					}

					else if(!strcmp(token2,"I"))			  // binary mode
					{
						asc_bin = 0;
						strcpy(result,"201 Type set to I\n");
					}
					else
						strcpy(result,"502 Type doesn't set\n"); 	// unknown type
				}

				else if(!strcmp(token, "STOR")) // receive file from client
				{
					data_size = 0;
					memset(temp, 0, MAX);
					if(asc_bin == 0) // binary mode
						sprintf(temp, "150 Opening binary mode connection for %s.\n", token2);
					if(asc_bin == 1) // ascii mode
						sprintf(temp, "150 Opening ascii mode connection for %s.\n", token2);
					write(client_fd, temp, strlen(temp));

					sprintf(temp, "%s [%s:%d] %s %s", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id, temp);
					write(log_fd, temp, strlen(temp)); sync();

					if(asc_bin == 0)
						file_des = open(token2, FLAGS, BIN_MODE);
					if(asc_bin == 1)
						file_des = open(token2, FLAGS, ASCII_MODE);

					while(1) // receive datas of file from client
					{
						memset(temp,0, MAX);
						read(to_client_data_fd, temp, MAX);


						if(strlen(temp) == 0) //finished transfer
							break;

						write(file_des, temp, strlen(temp)); // send file's inner data to client

					}

					close(file_des);
					write(client_fd, "226 Complete transmission\n",strlen("226 Complete transmission\n"));
					
					/* get received file size */
					data_size = lseek(file_des, 0, SEEK_END);

					memset(temp, 0, MAX);
					sprintf(temp, "%s [%s:%d] %s 226 Complete transmission | %d bytes\n", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id, data_size);
					write(log_fd, temp, strlen(temp));	sync();

					close(to_client_data_fd); // close data port to client
				}
				else if(!strcmp(token, "RETR")) // send file which client wants
				{
					data_size = 0;
					memset(temp, 0, MAX);

					if(asc_bin == 0) // binary mode
						sprintf(temp, "150 Opening binary mode connection for %s.\n", token2);
					if(asc_bin == 1) // ascii mode
						sprintf(temp, "150 Opening ascii mode connection for %s.\n", token2);
					write(client_fd, temp, strlen(temp));

					sprintf(temp, "%s [%s:%d] %s %s", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id, temp);
					write(log_fd, temp, strlen(temp)); sync();

					
					data_size = 0;	// transfer data size;

					file_des = open(token2, O_RDONLY);

					while(1)
					{
						memset(temp, 0, MAX);
						memset(temp2, 0 , MAX);
						n = read(file_des, temp2, MAX);
						if(asc_bin == 0) // binary mode
							strcpy(temp, temp2);
						else			  // ascii mode
							strcpy(temp, remove_rn(temp2));

						if(strlen(temp) == 0) // EOF
						{
							write(to_client_data_fd, temp, MAX);
							break;
						}
		
						data_size += (strlen(temp)*sizeof(char)); // adding sent data size 
						

						write(to_client_data_fd, temp, strlen(temp));

					}
					close(file_des);

					write(client_fd, "226 Complete transmission\n",strlen("226 Complete transmission\n"));
					
					/* get received file size */
					data_size = lseek(file_des, 0, SEEK_END);

					memset(temp, 0, MAX);
					sprintf(temp, "%s [%s:%d] %s 226 Complete transmission | %d bytes\n", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id, data_size);
					write(log_fd, temp, strlen(temp));	sync();

					close(to_client_data_fd);

				}
			 	
			 	else;
				
				if(data_trans_flag == 0)
				{
					write(client_fd, result, MAX);
					memset(temp, 0, MAX);
					sprintf(temp, "%s [%s:%d] %s %s\n", what_time(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), user_id, result);
					write(log_fd, temp, strlen(temp));	sync();
				}
			}
		}

		else  // parent process 
		{
			client_info(&client_addr); // print child process' infomation
			sprintf(temp,"Child Process ID : %d\n", pid);
			write(STDOUT_FILENO, temp, strlen(temp));

			/* make process node */
			cli_node = (Node*)malloc(sizeof(Node));
			cli_node->pid = pid;
			cli_node->port = ntohs(client_addr.sin_port);
			time(&cli_node->mytime);
			cli_node->client_pid = atoi(client_pid_arr);
			strcpy(cli_node->ip, inet_ntoa(client_addr.sin_addr));
			strcpy(cli_node->ID, user_id);

			/* insert node into linked list */
			if(list->child_count == 0)
			{
				list->phead = cli_node;
				list->ptail = cli_node;
			}
			else
			{
				list->ptail->next = cli_node;
				list->ptail = cli_node;
			}
			list->child_count++;
			print_process(); // print current child process
			alarm(10);// reset timer

		}

		close(client_fd);		// close client socket			
	} 

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// sh_int																//
// ==================================================================== //
// Input: int signum -> SIGINT signal									//
// Output: none															//
// Purpose: capture ctrl+C signal										//
//////////////////////////////////////////////////////////////////////////
void sh_int(int signal) // SIGINT handler
{
	Node* temp;
	Node* temp2;
	char arr[100];
	time_t current_time;
	time(&current_time);

	temp = list->phead;
	if(list->child_count != 0)
	for(;;)
	{

		if(temp == list->ptail)
		{

			kill(temp->pid, SIGTERM); // kill child process
			kill(temp->client_pid, SIGTERM); // kill client process
			free(temp); // diallocate last node(process)
			break;
		}
		
		temp2 = temp;

		kill(temp->pid, SIGTERM); // kill child process
		kill(temp->client_pid, SIGTERM);

		temp = temp->next;
		free(temp2);
	}
	write(STDOUT_FILENO, "\nExit Server\n", strlen("\nExit Server\n"));
	sprintf(arr, "%s Server is terminated.\n", what_time());
	write(log_fd, arr, strlen(arr));
	kill(getpid(), SIGTERM); // terminate server process
}

void sh_term(int signal)
{
	exit(0);
}

//////////////////////////////////////////////////////////////////////////
// sh_chld																//
// ==================================================================== //
// Input: int signum -> SIGCHLD signal									//
// Output: none															//
// Purpose: capture SIGCHLD signal and 				//											//
//////////////////////////////////////////////////////////////////////////
void sh_chld(int signal)
{
	Node* temp_node;
	Node* temp2_node;
	int child;
	char temp[100];
	time_t current_time;

	time(&current_time); // get current time

	child = wait(NULL); // recieve changed child process' pid


	/* diallocate child process' info node*/
	temp_node = list->phead; 
	for(;;)
	{
		if(temp_node->pid == child)
		{
			if(temp_node == list->phead) 
			{
				if(temp_node == list->ptail) // if node is the only one in list
					list->phead == NULL;
			
				else						 // not only one
					list->phead = temp_node->next;
					
				
				temp_node->next = NULL;
				sprintf(temp, "%s [%s:%d] %s LOGOUT | [Total Service Time : %d]\n", what_time(), temp_node->ip, temp_node->port, temp_node->ID, (int)(current_time-temp_node->mytime));
				write(log_fd, temp, strlen(temp)); sync();
				free(temp_node);
			}
			else if(temp_node == list->ptail)
			{
				list->ptail = temp2_node;
				temp2_node->next = NULL;
				sprintf(temp, "%s [%s:%d] %s LOGOUT | [Total Service Time : %d]\n", what_time(), temp_node->ip, temp_node->port, temp_node->ID, (int)(current_time-temp_node->mytime));
				write(log_fd, temp, strlen(temp)); sync();
				free(temp_node);
			}
			else
			{
				temp2_node->next = temp_node->next;
				temp_node->next = NULL;
				sprintf(temp, "%s [%s:%d] %s LOGOUT | [Total Service Time : %d]\n", what_time(), temp_node->ip, temp_node->port, temp_node->ID, (int)(current_time-temp_node->mytime));
				write(log_fd, temp, strlen(temp)); sync();
				free(temp_node);
			}

			break;
		}

		temp2_node = temp_node;
		temp_node = temp_node->next;
	}

	kill(child, SIGTERM); // kill child process
	list->child_count--;

	print_process(); // print child process
	alarm(10);		// reset timer
}


//////////////////////////////////////////////////////////////////////////
// sh_alrm																//
// ==================================================================== //
// Input: int signum -> SIGALRM signal									//
// Output: none															//
// Purpose: print child process and reset timer 						//
//////////////////////////////////////////////////////////////////////////
void sh_alrm(int signal)
{
	print_process();
	alarm(10);
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
	char temp[MAX];
	char ip[20] ;
	int port;
	int count=0;
	if(inet_ntoa(clientaddr->sin_addr) < 0) return -1;
	else strcpy(ip, inet_ntoa(clientaddr->sin_addr));
	if(ntohs(clientaddr->sin_port) < 0) return -1;
	else port = ntohs(clientaddr->sin_port);

	sprintf(temp, "========Client info========\nIP address : %s\n\nPort # : %d \n===========================\n", ip, port);
	write(STDOUT_FILENO, temp, strlen(temp)); // print input info
	return 0;
	
}


//////////////////////////////////////////////////////////////////////////
// print_process														//
// ==================================================================== //
// Input: none															//
// Output: none															//
// Purpose: print chlid process											//
//////////////////////////////////////////////////////////////////////////
void print_process()
{
	Node* point = list->phead;
	char arr[256];
	time_t current_time;

	time(&current_time); // get current time

	sprintf(arr, "Current Number of Client :  %d\nPID	  PORT	    TIME\n", list->child_count);
	write(STDOUT_FILENO, arr, strlen(arr));
	
	if(list->child_count == 0)
		return;

	/* print child process' pid, port number, access time*/
	while(1)
	{
		sprintf(arr, "%-10d%-10d%d\n", point->pid, point->port, (int)(current_time-point->mytime)); // time : current time - node's create time
		write(STDOUT_FILENO, arr, strlen(arr));
		
		if(point == list->ptail)
			break;
		point = point->next;
	}
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




//////////////////////////////////////////////////////////////////////////
// get_permi															//
// ==================================================================== //
// Input: char** oblist -> object list in the directory					//
// 			char* result -> will recieve result							//
//			int  ob_count -> object's count								//
// Output: none															//
// Purpose: get objects' permission and concatenate them into result	//
//////////////////////////////////////////////////////////////////////////

void get_permi(char oblist[50][50], char* result, int ob_count)
{
	int a;
	char buffer[MAX];

	struct stat st;		// status struct
	struct passwd *pw;	// password struct
	struct group *gr;	// group struct
	struct tm *t;		// time struct

	char permission[15]; // recieve object's permission


	for(a =0; a < ob_count; a++) // about all name in the array
   	{
   		memset(buffer, 0, sizeof(buffer));
		stat(oblist[a], &st); // get status about file
		pw = getpwuid(st.st_uid); // get password struct info
		gr = getgrgid(st.st_gid); // get group struct info
		t = localtime(&st.st_atime); // get time info 			

		// set user permission
		if(S_ISDIR(st.st_mode)) strcpy(permission, "d");
		else 			strcpy(permission, "-");
		if(st.st_mode & S_IRUSR)strcat(permission, "r");
		else 			strcat(permission, "-");
		if(st.st_mode & S_IWUSR)strcat(permission, "w");
		else			strcat(permission, "-");
		if(st.st_mode & S_IXUSR)strcat(permission, "x");
		else 			strcat(permission, "-");

		// set group permission
	    if(st.st_mode & S_IRGRP)strcat(permission, "r");
	    else                    strcat(permission, "-");
	    if(st.st_mode & S_IWGRP)strcat(permission, "w");
	    else                    strcat(permission, "-");   
	    if(st.st_mode & S_IXGRP)strcat(permission, "x");       
	    else                    strcat(permission, "-");     

	    //set other permission
		if(st.st_mode & S_IROTH)strcat(permission, "r");             
	    else                    strcat(permission, "-");           
	    if(st.st_mode & S_IWOTH)strcat(permission, "w");       
	    else                    strcat(permission, "-");       
	    if(st.st_mode & S_IXOTH)strcat(permission, "x");      
	    else                    strcat(permission, "-");   

	    /* save result */
		sprintf(buffer, "%s %d %s %s %d %d %d %d:%d %s",permission, st.st_nlink, pw->pw_name, gr->gr_name, st.st_size, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, oblist[a]);
		strcat(result, buffer);

		if(S_ISDIR(st.st_mode))
			strcat(result, "/");// plus '/' to directory
		strcat(result, "\n");

	}

}


//////////////////////////////////////////////////////////////////////////////
// log_auth																	//
// ======================================================================== //
// Input: int connfd -> client socket discriptor							//
//		  struct sockaddr_in* clientaddr -> to get IP address				//
// Output: 1(permited User)	/	0(Unpermited User)							//
// Purpose: recieve User ID/Passwd from client and then		 				//
//			confirm who want to connect with server is permited User or not	//
//////////////////////////////////////////////////////////////////////////////

int log_auth(int connfd, struct sockaddr_in* clientaddr)
{
	char user[MAX];
	char temp[MAX];
	char id[20];
	char *token1, *token2, *token3;
	int n, count = 1;

	while(1)
	{

		memset(temp, 0, sizeof(temp));
		memset(user, 0, sizeof(user));

		sprintf(temp, "User is trying to log-in (%d/3)\n", count);
		write(STDOUT_FILENO, temp, strlen(temp));

		/* recieve username from client*/
		read(connfd, user, MAX);
		token1 = strtok(user, " "); // ID
		if(!strcmp(token1, "USER"))
		{
			token2 = strtok(NULL, " "); // get id
			if((n = user_match(token2))==1) // compare ID(find matched User name)
			{
				strcpy(id, token2);
				memset(temp, 0, sizeof(temp));
				sprintf(temp, "331 Password is required for %s\n", id);
				write(connfd, temp, strlen(temp));

				memset(user, 0, sizeof(user));
				read(connfd, user, MAX);		// receive passwd
				token1 = strtok(user, " ");


				if(!strcmp(token1,"PASS"))
				{
					token2 = strtok(NULL, " ");
					n = pass_match(id, token2);

					if(n == 1) // compare PASSWD(find matched passwd) // insert username, passwd
					{
						memset(temp, 0, sizeof(temp));
						sprintf(temp, "230 User %s logged in\n", id);
						strcpy(user_id, id);
						write(connfd, temp, strlen(temp));

						memset(temp, 0, sizeof(temp));
						sprintf(temp, "%s [%s:%d] %s LOG_IN\n", what_time(),inet_ntoa(clientaddr->sin_addr), ntohs(clientaddr->sin_port), id); // log
						write(log_fd, temp, strlen(temp)); sync();
						break;
					}
				}

			}
			else // Illegal user case
			{
				memset(temp, 0, sizeof(temp));
				sprintf(temp, "%s [%s:%d] %s LOG_FAIL\n", what_time(),inet_ntoa(clientaddr->sin_addr), ntohs(clientaddr->sin_port), token2);
				write(log_fd, temp, strlen(temp)); sync();
			}

			if(count >= 3)// failed more than 3 times, disconnect client
			{
				write(connfd, "530 Failed to Log-in\n", strlen("530 Failed to Log-in\n"));
				return 0;

			}
			//forgive it about 3 times
			write(connfd, "430 Invalied username or passwd\n", strlen("430 Invalied username or passwd\n"));
			write(STDOUT_FILENO, "** Log-in failed **\n", strlen("** Log-in failed **\n"));
			count++;
		}
	}
	return 1;

}

//////////////////////////////////////////////////////////////////////////
// user_match															//
// ==================================================================== //
// Input: char *user -> recieved user Id 								//
// Output: 0(Unpermited User)	/	1(permited User)					//
// Purpose: check received ID is permitted or not 						//
//////////////////////////////////////////////////////////////////////////

int user_match(char *user)
{
	FILE* fp;
	int flag = 0;
	struct passwd *pw;
	fp = fopen("passwd", "r");

	while(!feof(fp))

	{
		pw = fgetpwent(fp);

		if(!strcmp(user, pw->pw_name)) // found matched user name
		{
			flag=1;						//setflag
			break;
		}

		
	}

	fclose(fp);
	if(flag == 0)
		return 0;
	else
		return 1;
}

//////////////////////////////////////////////////////////////////////////
// pass_match															//
// ==================================================================== //
// Input: char *user -> recieved user ID 								//
//			char *password -> received password 						//
// Output: 0(Unpermited User)	/	1(permited User)					//
// Purpose: check received ID's password is permitted or not 			//
//////////////////////////////////////////////////////////////////////////
int pass_match(char *user, char *password)
{
	FILE* fp;
	int flag = 0;
	struct passwd *pw;
	fp = fopen("passwd", "r");

	while(!feof(fp)) // find user name
	{
		pw = fgetpwent(fp);

		if(!strcmp(user, pw->pw_name))
		{
			if(!strcmp(password, pw->pw_passwd)) // compare password
				flag = 1;
			
			break;
		}
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

		flag = 1; // if all ip components are matched, set flag into 1
		break;
	}
	fclose(fp);

	if(flag == 1)
		return 1;

	else 
		return 0;

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
	char *token, *token2, *token3, *token4, *token5;
	char string[MAX];
	strcpy(string, str);

	addr = malloc(100*sizeof(char));


	// seperate ip address
	token = strtok(str, ",");
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
// what_time															//
// ==================================================================== //
// Input: none 															//
// Output: char *ctime 													//
// Purpose: return current time 										//
//////////////////////////////////////////////////////////////////////////
char* what_time()
{
	char *ctime;
	char week[7][5] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
	char mon[12][5] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	struct tm *t;
	time_t timer;	//for  time  check

	ctime = malloc(50*sizeof(char));	

	timer = time(NULL);
	t = localtime(&timer);

	sprintf(ctime, "%s %s %d %d:%d:%d %d", week[t->tm_wday], mon[t->tm_mon], t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, t->tm_year+1900);

	return ctime;
}
//////////////////////////////////////////////////////////////////////////
// welcome																//
// ==================================================================== //
// Input: int sock_fd -> connected client 								// 															//
// Output: none 														//
// Purpose: send motd's data with current time 							//
//////////////////////////////////////////////////////////////////////////

void welcome(int sock_fd)
{
	FILE* fd;
	char temp[MAX];
	char result[MAX];
	char *token;
	fd = fopen("motd", "r");
	memset(result, 0, MAX);
	fgets(temp, MAX, fd);

	token = strtok(temp, "%");
	strcat(result, token);
	strcat(result, what_time());
	strcat(result, ")\n");

	write(sock_fd, result, strlen(result));
}

//////////////////////////////////////////////////////////////////////////
// remove_rn															//
// ==================================================================== //
// Input: char *str -> data which send to client 						//
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
		else
		{
			addr[a] = str[i];
			i++;
			a++;
		}
		if(i > strlen(str)) break;
	}

	return addr;
}