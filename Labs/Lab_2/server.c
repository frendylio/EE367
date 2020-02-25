/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3508"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
     int numBytes; //number of bytes received from client
     int fd[2]; //Piping
     char bigBuf[1000]; //For sending data
     char bigArg[1000];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

          //Connection successful
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);
         
          //Fork to handle connected client
          if(!fork()) {
               close(sockfd);
               //Get command from client
               numBytes = recv(new_fd,bigBuf,sizeof(bigBuf),0);
               bigBuf[numBytes] = '\0';
               printf("SERVER RECEIVED:\n'%s'",bigBuf);
               switch(bigBuf[0]) {
                    case 'l':
                         pipe(fd);
                         if(!fork()) { //execute ls command
                              close(fd[0]);
                              dup2(fd[1], STDOUT_FILENO);  
                              execlp("/bin/ls","-a",(char*)NULL);
                         }
                         //Process output
                         close(fd[1]);
                         if((numBytes = read(fd[0],bigBuf,sizeof(bigBuf)))>0) {
                         bigBuf[numBytes] = '\0';
                         send(new_fd,bigBuf,strlen(bigBuf),0);
                         exit(0);
                         }
                         break; //This shouldn't run

                    case 'c':
                         memmove(bigBuf, bigBuf+1, strlen(bigBuf)); //Get rid of command portion (only keep file na
                         //Check if file is in current path
                         if(access(bigBuf,F_OK)!=0) {
                              send(new_fd,"File does not exist",20,0);
                         }
                         else {
                              send(new_fd,"File exists",13,0);
                         }
                         exit(0);
                         break;
                    case 'p':
                         memmove(bigBuf, bigBuf+1, strlen(bigBuf)); //Get rid of command portion (only keep file name)
                         //Display contents of file if exists
                         if(access(bigBuf,F_OK)==0) {
                              pipe(fd);
                              if(!fork()) {
                                   close(sockfd);
                                   close(fd[0]);
                                   dup2(fd[1], STDOUT_FILENO);
                                   execlp("/bin/cat","cat",bigBuf,(char*)NULL);
                              }
                              close(fd[1]);
                              while((numBytes=read(fd[0],bigBuf,sizeof(bigBuf)))>0) {
                                   printf("GOOD");
                                   bigBuf[numBytes] = '\0';
                                   send(new_fd,bigBuf,strlen(bigBuf),0);
                              }
                              exit(0);
                              break;
                         }
                         else {
                              send(new_fd,"File does not exist",20,0);
                         }
                         exit(0);
                         break;

                    case 'd':
                         memmove(bigBuf, bigBuf+1, strlen(bigBuf)); //Get rid of command portion (only keep file name)
                         //Display contents of file if exists
                         if(access(bigBuf,F_OK)==0) {
                              send(new_fd,"1",2,0); //indicates that file exists
                              pipe(fd);
                              if(!fork()) {
                                   close(sockfd);
                                   close(fd[0]);
                                   dup2(fd[1], STDOUT_FILENO);
                                   execlp("/bin/cat","cat",bigBuf,(char*)NULL);
                              }
                              close(fd[1]);
                              while((numBytes = read(fd[0],bigBuf,sizeof(bigBuf))) >0) {
                                   bigBuf[numBytes] = '\0';
                                   send(new_fd,bigBuf,strlen(bigBuf),0);
                              }
                              exit(0);
                              break;
                         }
                         else { //Indicates that file doesn't exist
                              send(new_fd,"0",2,0); //indicate that file doesn't exist
                         }
                         exit(0);
                         break;

                    default: send(new_fd,"Did not recognize",20,0);
                             exit(1);

          }
               //This shouldn't ever run
               printf("???");
               exit(0);
     }

		close(new_fd);  // parent doesn't need this
     }

	return 0;
}


