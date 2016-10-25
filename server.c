#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

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
      printf("%s",buffer);
      if(len==0) { break; }
   }

   close(client_fd);
   close(sock_fd);
   
}
