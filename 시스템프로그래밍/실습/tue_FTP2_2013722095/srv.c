//////////////////////////////////////////////////////////////////////////
// File Name : srv.c													//
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

#define MAX 4096
void sh_chld(int); // signal handler for SIGCHLD
void sh_alrm(int); // signal handler for SIGALRM
void sh_int(int); // signal handler for SIGINT
void sh_term(int);
int client_info(struct sockaddr_in*); // print client's info
void print_process();
void sort_func(char(*)[50], int);
void get_permi(char(*)[50], char*, int);


typedef struct Node
{
	int pid;
	int port;
	int client_pid;
	struct Node* next;
	time_t	mytime;

}Node;

typedef struct List
{
	struct Node* phead;
	struct Node* ptail;
	int child_count;
}List;


List* list;

int main(int argc, char** argv)
{
	/* socket variable */
	int n;
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;
	int len;
	int port;
	char client_pid_arr[MAX];

	char buffer[MAX]; // recieve system buffer here
	char result[MAX];	// command excute result
	char temp[MAX];	// temp
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


	/* dynamic allocation of list*/
	list = (List*)malloc(sizeof(List));
	list->phead = NULL;
	list->ptail = NULL;
	list->child_count = 0;


	Node* cli_node;
	Node* temp_node;
	Node* temp2_node;


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
				token = NULL;
				token2 = NULL;
				token3 = NULL;
				ob_count = 0;
				D_flag = 0;

				/* recieve message from client*/
				n = read(client_fd, buffer, MAX);
				buffer[n-1] = ' ';

				sprintf(temp,"%-s [%d]\n\n", buffer, getpid());
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
					getcwd(result, MAX);
					strcat(result, "\n");
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
						getcwd(result, MAX);
						strcat(result,"<- current working directory\n");

					}
					else if(chdir(token2) != 0) // have additional path and it was wrong 
					{	
						strcpy(result, "error : wrong path\n");}
					else
					{
						getcwd(result, MAX);
						strcat(result,"<- current working directory\n");
					}
				}
				
				else if(!strcmp(token, "CDUP"))
				{
					chdir("..");
			        getcwd(result, 50);
					strcat(result,"<- current working directory\n");
				}

			    // make new directory
				else if(!strcmp(token, "MKD"))
				{
					umask(0); // set umask to zero
			 		for(a =0; a < ob_count; a++)
					{
			 			if(mkdir(oblist[a],0744) != 0) // if failed to make directory
						{
							sprintf(temp, "Failed to make DIR: %s\n", oblist[a]);
						 	strcat(result, temp);
						 	D_flag = 1;
						}
					}

					if(D_flag == 0)
						strcpy(result, "Success!\n");
					
				}

				// delete object(file, directory)
				else if(strcmp(token, "DELE")==0)
				{
			        for(a =0; a < ob_count; a++)
			        {
			            if(remove(oblist[a]) != 0) // if failed to make directory
			            {
			        	   	sprintf(temp, "Failed to remove object: %s\n", oblist[a]);
			              	strcat(result, temp);
			               	D_flag = 1;
			            }			                
			        }

			        if(D_flag == 0)
			        	strcpy(result, "Success!\n");
				}

				// remove directory
				else if(strcmp(token, "RMD")==0)
				{
					for(a =0; a < ob_count; a++)
			        {
			            if(rmdir(oblist[a]) == -1) // if failed to make directory
			            {
			            	sprintf(temp, "Failed to remove DIR: %s\n", oblist[a]);
			                strcat(result, temp);
			                D_flag = 1;
			            }
			        }

			        if(D_flag == 0)
			        	strcpy(result, "Success!\n");
				}
				
				else if(strcmp(token, "RNFR")==0) // rename:oldname
				{
					dp = opendir("."); // open current directory
					
					while(dirp = readdir(dp))
					{ 
						if(!strcmp(token2,dirp->d_name)) 
							flag = 1;
					} // if program found input named file, set flag as 1
					
					if(flag == 0) // couldn't find file
					{
						sprintf(result, "Couldn't find that object: %s\n", token2);
					}
					else
					{
						// rename file into newname
						token3 = strtok(NULL, " "); 
						
						if(rename(token2, token3) != 0) // if newnamed file is already exist, error
						{
							sprintf(result, "%s is already exist\n", token3);
						}
						else
						{
							strcpy(result, "Success!\n");
						}

					}

				}

				// Quit
				else if(strcmp(token, "QUIT")==0)
				{
					exit(1); // close current child process 
				}
			 	
			 	else;
				
				write(client_fd, result, MAX);

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
	} // end of while(1)

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
				free(temp_node);
			}
			else if(temp_node == list->ptail)
			{
				list->ptail = temp2_node;
				temp2_node->next = NULL;
				free(temp_node);
			}
			else
			{
				temp2_node->next = temp_node->next;
				temp_node->next = NULL;
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
		sprintf(arr, "%-10d%-10d%d\n", point->pid, point->port, (current_time-point->mytime)); // time : current time - node's create time
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