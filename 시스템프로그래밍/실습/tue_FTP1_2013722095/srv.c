//////////////////////////////////////////////////////////////////////////
// File Name : srv.c							//
// Date: 2017.04.28							//
// Os: Ubuntu 12.04 LTS 64bits						//
// Author: CHoi Jae eun							//
// Student ID: 2013722095 						//
// -----------------------------------------------------------------	//
// Title: System Programming Assignment #2 (ftp server) 		//
// Description : server part code					//
//////////////////////////////////////////////////////////////////////////


#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(void)
{
	char buffer[200]; // recieve system buffer here
	char temp[50];	// temp
	char com1[50];	// use for compare
	char com2[50];	// use for compare
	char oblist[20][20]; // save objects name in a directory
	char permission[12]; // recieve object's permission
	char error[200];     // copy buffer array here
	char* token;	    
	char* token2;
	char* token3;
	DIR *dp;		// directory pointer
	struct dirent *dirp;	// object pointer in a directory
	struct stat st;		// status struct
	struct passwd *pw;	// password struct
	struct group *gr;	// group struct
	struct tm *t;		// time struct
	time_t time;		
	int ob_count = 0;	// object counter
	int a, b, c =0;
	int flag =0;		// flag for RNFR & RNTO

	// seperate command, option, path from buffer
	read(0, buffer, 200);
	strcpy(error, buffer);
	token = strtok(buffer, " ");
	if(!strcmp(token, "error")) { write(1, error, strlen(error)); return 0;}  // if sentence is start with 'error' word , print whole sentence and close program

	// for under command, count its additional objects and copy them into another 2D array
	if((strcmp(token, "MKD") ==0) || (strcmp(token, "RMD")==0) || (strcmp(token, "DELE") ==0))
	{
		while(1) // count object's number and save its name
		{
			token2 = strtok(NULL, " ");
			if(token2 == NULL) break;
			strcpy(oblist[ob_count], token2);
			ob_count++;
		}
	}
	else // in other command case, cut and set token like under that
	{
	token2 = strtok(NULL, " ");
	token3 = strtok(NULL, " ");
	}
	write(1, error, strlen(error)); // print input command, option, path
	write(1, "\n", strlen("\n"));
	if(strcmp(token, "NLST")==0) // if command is NLST(ls)
	{
	    if((token2 == NULL) && (token3 == NULL)) // no option, no path
	    {
                        dp = opendir("."); // open current directory
                           while(dirp = readdir(dp)) // save objects' name into array except some object which start with .
                           {   strcpy(temp, dirp->d_name);
				if(temp[0] == '.') continue;
				strcpy(oblist[ob_count], dirp->d_name); ob_count++; }
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
                           for(a = 1; a <= ob_count; a++) // print all object
                           {
                                write(1, oblist[a-1], strlen(oblist[a-1]));
                                write(1, "  ", strlen("  "));
                                if(a%5 == 0)   write(1, "\n", strlen("\n"));
                           }
 
	    }


	    if((token2 != NULL) && (token3 == NULL) )  // if input has only one option or a path
	    {
		if((strcmp(token2, "-a") !=0) && (strcmp(token2, "-l")!=0) && (strcmp(token2, "-al")!=0)) // buffer has only path without option
		{   dp = opendir(token2); // open 'path'  directory  
		    while(dirp = readdir(dp)) // colander files which start with '.'
		    {  strcpy(temp, dirp->d_name);
			if(temp[0] == '.') continue;
			strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
		    }


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
                           for(a = 1; a <= ob_count; a++) // print objects
                           {
                                write(1, oblist[a-1], strlen(oblist[a-1]));
                                write(1, "  ", strlen("  "));
                                if(a%5 == 0)   write(1, "\n", strlen("\n"));
                           }
    
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		else // buffer has only option(list of cwd)
		{
			dp = opendir("."); // open current directory 

			if(strcmp(token2, "-a") ==0) // in -a option
			{
			   while(dirp = readdir(dp)) // save all files' name into array
                	   {  strcpy(oblist[ob_count], dirp->d_name); ob_count++; }
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
                           for(a = 1; a <= ob_count; a++) // print object
                           {
				write(1, oblist[a-1], strlen(oblist[a-1]));
				write(1, "  ", strlen("  "));
				if(a%5 == 0)   write(1, "\n", strlen("\n"));
			   }
                     	}
                        if(strcmp(token2, "-l") ==0) // with -l option
                        {
                           while(dirp = readdir(dp))
                           { 
				strcpy(temp, dirp->d_name);
				if(temp[0] == '.') continue; // won't get hidden files
			     	strcpy(oblist[ob_count], dirp->d_name); ob_count++; 
			   }
	
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

			    for(a =0; a < ob_count; a++) // about all name in the array
			    {
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
			    
				sprintf(buffer, "%s %d %s %s %d %d %d %d:%d %s",permission, st.st_nlink, pw->pw_name, gr->gr_name, st.st_size, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, oblist[a]);
				write(1, buffer, strlen(buffer));
				if(S_ISDIR(st.st_mode)) write(1, "/", strlen("/")); // plus '/' to directory
				write(1, "\n", strlen("\n")); 
			    }
			}

                        if(strcmp(token2, "-al") ==0) // with -al option
                        {
                           while(dirp = readdir(dp)) 
                           { strcpy(oblist[ob_count], dirp->d_name); ob_count++; } // get all files name
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

			    for(a =0; a < ob_count; a++)
			    {
				//get current files info (status, password, group, time)
				stat(oblist[a], &st);
				pw = getpwuid(st.st_uid);
				gr = getgrgid(st.st_gid);
				t = localtime(&st.st_atime);			
	
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
			    
				sprintf(buffer, "%s %d %s %s %d %d %d %d:%d %s",permission, st.st_nlink, pw->pw_name, gr->gr_name, st.st_size, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, oblist[a]);
				write(1, buffer, strlen(buffer));
				if(S_ISDIR(st.st_mode)) write(1, "/", strlen("/")); // add '/' to directory name
				write(1, "\n", strlen("\n")); 
			    }
			}

		}
	 }
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if((token2 != NULL) && (token3 != NULL)) // input has option and path
	{ 
			write(1, token3, strlen(token3));
			dp = opendir(".");
//			dp = opendir(token3);  // open 'path' directory
			if(strcmp(token2, "-a") ==0) // with -a option
			{
			   while(dirp = readdir(dp)) // get all files name
                	   {  strcpy(oblist[ob_count], dirp->d_name); ob_count++; }
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
                           for(a = 1; a <= ob_count; a++) // print files name
                           {
				write(1, oblist[a-1], strlen(oblist[a-1]));
				write(1, "  ", strlen("  "));
				if(a%5 == 0)   write(1, "\n", strlen("\n"));
			   }
                     	}
                        if(strcmp(token2, "-l") ==0) // with -l option
                        {
                           while(dirp = readdir(dp))
                           { strcpy(temp, dirp->d_name);
			     if(temp[0] == '.') continue; // won't get hidden files
			     strcpy(oblist[ob_count], dirp->d_name); ob_count++; }
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

			    for(a =0; a < ob_count; a++) // about all files in the array, get its info(status, password, group, time)
			    {
				stat(oblist[a], &st);
				pw = getpwuid(st.st_uid);
				gr = getgrgid(st.st_gid);
				t = localtime(&st.st_atime);			
	
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
			    
				sprintf(buffer, "%s %d %s %s %d %d %d %d:%d %s",permission, st.st_nlink, pw->pw_name, gr->gr_name, st.st_size, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, oblist[a]);
				write(1, buffer, strlen(buffer)); // print info
				if(S_ISDIR(st.st_mode)) write(1, "/", strlen("/"));
				write(1, "\n", strlen("\n")); 
			    }
			}

                        if(strcmp(token2, "-al") ==0) // with -al option
                        {
                           while(dirp = readdir(dp))
                           { strcpy(oblist[ob_count], dirp->d_name); ob_count++; } // get all file name
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

			    for(a =0; a < ob_count; a++) // about all files, get its info(status, password, group, time)
			    {
				stat(oblist[a], &st);
				pw = getpwuid(st.st_uid);
				gr = getgrgid(st.st_gid);
				t = localtime(&st.st_atime);			
	
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
			    
				sprintf(buffer, "%s %d %s %s %d %d %d %d:%d %s",permission, st.st_nlink, pw->pw_name, gr->gr_name, st.st_size, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, oblist[a]);
				write(1, buffer, strlen(buffer)); // print info
				if(S_ISDIR(st.st_mode)) write(1, "/", strlen("/"));
				write(1, "\n", strlen("\n")); 
			    }
			}

		}
	 	

	closedir(dp); // close opened directory
	}



	else if(strcmp(token, "LIST")==0) // if command is LIST
	{
			if(token2 == NULL) dp = opendir(".");
			else 		   dp = opendir(token2);

                           while(dirp = readdir(dp))
                           { strcpy(oblist[ob_count], dirp->d_name); ob_count++; } // get all file name
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

                            for(a =0; a < ob_count; a++) // about all files, get its info(status, password, group, time)
                            {
                                stat(oblist[a], &st);
                                pw = getpwuid(st.st_uid);
                                gr = getgrgid(st.st_gid);
                                t = localtime(&st.st_atime);

                                // set user permission
                                if(S_ISDIR(st.st_mode)) strcpy(permission, "d");
                                else                    strcpy(permission, "-");
                                if(st.st_mode & S_IRUSR)strcat(permission, "r");
                                else                    strcat(permission, "-");
                                if(st.st_mode & S_IWUSR)strcat(permission, "w");
                                else                    strcat(permission, "-");
                                if(st.st_mode & S_IXUSR)strcat(permission, "x");
                                else                    strcat(permission, "-");
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

                                sprintf(buffer, "%s %d %s %s %d %d %d %d:%d %s",permission, st.st_nlink, pw->pw_name, gr->gr_name, st.st_size, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, oblist[a]);
                                write(1, buffer, strlen(buffer)); // print info
                                if(S_ISDIR(st.st_mode)) write(1, "/", strlen("/"));
                                write(1, "\n", strlen("\n"));
                            }


	}
	
	// print current working directory
	else if(strcmp(token, "PWD")==0){
	getcwd(temp, 50);
	write(1, temp, strlen(temp));
	}

	// move current working dirctory
	else if(strcmp(token, "CWD")==0){
	if(chdir(token2) != 0)
		{write(1, "Unknown Path\n", strlen("Unknown Path\n")); return 0;}
	getcwd(temp, 50);
	write(1,temp, strlen(temp));
	write(1, "<- cwd\n", strlen("<- cwd\n"));
	}
	else if(strcmp(token, "CDUP")==0){
	chdir("..");
        getcwd(temp, 50);
        write(1,temp, strlen(temp));
        write(1, "<- cwd\n", strlen("<- cwd\n"));
	
	}

        // make new directory
	else if(strcmp(token, "MKD")==0){
	umask(0); // set umask to zero
 	 for(a =0; a < ob_count; a++)
	 {
 		if(mkdir(oblist[a],0744) != 0) // if failed to make directory
		{sprintf(temp, "Failed to make DIR: %s\n", oblist[a]);
		write(1, temp, strlen(temp));}
	 }
	}

	// delete object(file, directory)
	else if(strcmp(token, "DELE")==0){
        for(a =0; a < ob_count; a++)
         {
                if(remove(oblist[a]) != 0) // if failed to make directory
                {sprintf(temp, "Failed to remove object: %s\n", oblist[a]);
                write(1, temp, strlen(temp));}
         }

	}

	// remove directory
	else if(strcmp(token, "RMD")==0){
	for(a =0; a < ob_count; a++)
         {
                if(rmdir(oblist[a]) != 0) // if failed to make directory
                {sprintf(temp, "Failed to remove DIR: %s\n", oblist[a]);
                write(1, temp, strlen(temp));}
         }
	}
	
	else if(strcmp(token, "RNFR")==0) // rename:oldname 
	{
		dp = opendir("."); // open current directory
		while(dirp = readdir(dp))
		{ if(strcmp(token2,dirp->d_name)==0) flag = 1;} // if program found input named file, set error flag
		if(flag == 0) // couldn't find file
		 {sprintf(temp, "Couldn't find that object: %s\n", token2);
		  write(1, temp, strlen(temp));return 0;}

		// rename file into newname
		token3 = strtok(NULL, " "); 
		if(rename(token2, token3) != 0) // if newnamed file is already exist, error
		{sprintf(temp, "%s is already exist\n", token3);
		 write(1, temp, strlen(temp));}
	}
	// Quit
	else if(strcmp(token, "QUIT")==0){
	write(1, "Quit program", strlen("Quit program"));	}
 	else write(1,"Unknown Command", strlen("Unknown Command"));
	
	write(1,"\n", strlen("\n"));	
return 0;
}


