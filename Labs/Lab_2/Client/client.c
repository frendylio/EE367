/*
** client-CHAN.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h> //For playing around with print statements
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3508" // the port client will be connecting to 

#define MAXDATASIZE 1000 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int attemptConnect(char *host)
{
	int sockfd, numbytes;  
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(host, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "\nCannot connect to %s\n",s);
		return -1;
	}
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);

	freeaddrinfo(servinfo); // all done with this structure
     
     return sockfd;
}

int main(int argc, char *argv[]) {
	char buf[MAXDATASIZE];
     char *arg[2]; //Used for any command arguments
     char *host;
     int sockfd;
     int isServer=0;
     int numbytes;
     
     char newFileName[100]; //For downloading files
     char confirm;
     FILE * fp;

     if(argc !=2) {
          printf("\nCorrect usage: client <hostname>\n");
          return -1;
     }
     host = argv[1]; //For connecting to server

     /* Functionality of CLIENT-CHAN:
      * (1) Client takes in a command from the user
      * (2) If the command requires information from the server:
      *   - Client attempts to connect to server
      *   - If successful, client sends over encoded command
      *   - Server sends reply to client
      *   - Client outputs reply
      * (3) If command does not require information:
      *   - Client processes request
      * (4) Client awaits another command from the  user 
      */

     while(1) { //infinite loop
          printf("\n==================================\n");
          printf("Awaiting command (h for help): ");
          scanf("%[^\n]%*c",buf);
          printf("\n==================================\n");
          //Encode the command
          switch(buf[0]) {
               case 'l': //Return server directory files
                    isServer = 1; //Bit indicates whether to establish connection with server
                    buf[1] = '\0';
                    break;
               case 'c':
                    isServer = 1;
                    arg[0] = strtok(buf," ");
                    arg[0][1] = '\0';
                    arg[1] = strtok(NULL," ");
                    if(arg[1] == NULL) {
                         printf("\nCorrect usage: command <filename>\n");
                         continue;
                    }
                    strcat(arg[0],arg[1]);
                    break;

               case 'd': //Download specified file
                    if(buf[1] == 'i') {} //User must want display, skip this case and goto display case
                    else {
                         isServer = 0; //We set this to 0 because we will be handling server connection within this case statement
                         arg[0] = strtok(buf," ");
                         arg[0][1] = '\0';
                         arg[1] = strtok(NULL," ");
                         if(arg[1] == NULL) {
                              printf("\nCorrect usage: command <filename>\n");
                              continue;
                         }
                         //Need to do some additional handling to save file
                         //(1) Check if file already exists
                         //(2) Save output to file if doesn't exist
                         if(access(arg[1],F_OK)==0) { //if file exists...
                             printf("\nA file with that name already exists\n"); 
                             printf("Would you like to overwrite? (y/n)\n");
                             if(getchar() != 'y') {
                                  //Buffer
                                  while(getchar() != '\n') {}
                                  continue;
                             }
                             while(getchar() != '\n') {} //getchar is boosted, buffer needed here too
                         }
                         printf("arg[0]: %s\n",arg[1]);
                         memcpy(newFileName,arg[1],strlen(arg[1])+1);
                         //Establish connection with server, get file info
                         strcat(arg[0],arg[1]);
                         if((sockfd = attemptConnect(host))==-1) {
                              perror("Could not connect to server");
                              continue;
                         }
                         //Send file & command name
                         send(sockfd,buf,strlen(buf),0);
                         //Receive file info from server
                         if((numbytes = recv(sockfd,buf,MAXDATASIZE,0)) <= 0) {
                              perror("\nDid not receive anything\n");
                              continue;
                         }
                         else {
                              buf[numbytes] = '\0';
                         }
                         if(memcmp("0",buf,strlen("0"))==0) {//means no file exists
                              printf("\nit looks like this file doesn't exist on server\n");
                         }
                         else { //if file exists, save to clientside
                              //w flag opens new file, if file exists, overwrites old one
                              fp = fopen(newFileName, "w");
                              if(fp!=NULL) {
                                   while((numbytes = recv(sockfd,buf,MAXDATASIZE,0)) > 0) {
                                        //uneeded print statements
                                        buf[numbytes] = '\0';
                                        printf("%s",buf);
                                        //store in file
                                        fprintf(fp, "%s",buf);
                                   }
                              }
                              fclose(fp);
                         }
                         break;
                    }
               case 'p': //Display contents of specified file
                    isServer = 1;//We set this to 0 because we will be handling server connection within this case statement
                    arg[0] = strtok(buf," ");
                    arg[0][1] = '\0';
                    arg[1] = strtok(NULL," ");
                    if(arg[1] == NULL) {
                         printf("\nCorrect usage: command <filename>\n");
                         continue;
                    }
                    arg[0][0] = 'p';
                    arg[0][1] = '\0';
                    strcat(arg[0],arg[1]);
                    break;
               case 'h': //Help command
                    isServer = 0;
                    printf("l: list\nc: Check <filename>\np: Display <filename>\nd: Download <filename>\nq: Quit\n\n");
                    break;
               case 'q': //Quit the program
                    printf("\n\nBye Bye!\n");
                    return 0;
                    break;
               default: //Handling for invalid command
                    isServer = 0;
                    printf("\nInvalid Command\n");
                    break;
          }
          if(isServer) {
               if((sockfd = attemptConnect(host))==-1) {
                    perror("Could not connect to server");
               }
               else {
                    send(sockfd,buf,strlen(buf),0);
                    printf("\n$$$$$$$$$$$$$$OUTPUT$$$$$$$$$$$$$$\n");
                    while((numbytes = recv(sockfd,buf,MAXDATASIZE,0)) > 0) {
                         buf[numbytes] = '\0';
                         printf("%s",buf);
                    }
                    printf("\n$$$$$$$$$$$$$$OUTPUT$$$$$$$$$$$$$$\n");
               }
          }

     }
          close(sockfd);
}
