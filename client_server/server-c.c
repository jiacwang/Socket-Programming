#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
*/
int server(char *server_port) {
  
  // server port is the port number to listen on for client connections
    int sockfd, new_fd;
    int numbytes;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int yes = 1;
    int rv;
    char buf[RECV_BUFFER_SIZE];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
  
    if ((rv = getaddrinfo("127.0.0.1", server_port, &hints, &servinfo)) != 0){

      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;

    }
    

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
      
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof yes) == -1) {
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

    freeaddrinfo(servinfo); 

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, QUEUE_LENGTH) == -1) {
        perror("listen");
        exit(1);
    }

    // main accept() loop
    while(1) {  
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
        
        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            // recv multiple times to get all the data
            while ((numbytes = recv(new_fd, buf, RECV_BUFFER_SIZE, 0)) > 0) {
                printf("%s", buf);
                fflush(stdout);
                memset(&buf, 0, sizeof buf);  
            }
            
            close(new_fd);
            exit(0);
        }      
        close(new_fd);  // parent doesn't need connection 
           
    }
    return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv) {
  char *server_port;
  //int yes = 1;
  //setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
  
  if (argc != 2) {
    fprintf(stderr, "Usage: ./server-c [server port]\n");
    exit(EXIT_FAILURE);
  }

  server_port = argv[1];
  // add error checking for port number
  int port_num = atoi(server_port);

  if (port_num < 10000 || port_num > 60000) {
    fprintf(stderr, "port number is wrong\n");
    exit(EXIT_FAILURE);
  }
  return server(server_port);
}
