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

#define SEND_BUFFER_SIZE 2048


/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/

int partial_send(int socket, char *buffer, int *length)
{
    int sent = 0; 
    int return_value;   
    int remaining_bytes = *length;
    
    while (sent < *length) {
        return_value = send(socket, buffer+sent, remaining_bytes, 0);
        if (return_value == -1) { break; }
        sent += return_value;
        remaining_bytes -= return_value;
    }

    *length = sent; 

    return return_value==-1?-1:0; 
} 

int client(char *server_ip, char *server_port) {

    int sockfd, numbytes;
    char buf[SEND_BUFFER_SIZE];
    
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(server_ip, server_port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror (rv));
        return 1;
    }
    
    //loop through all the results and connect to the first we can

     for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    
    //each time we read SEND_BUFFER_SIZE, and send it, until we reach EOF
    int read_flag; 
    do {
        read_flag = read(0, buf, SEND_BUFFER_SIZE);
        
        int len;
        len = strlen(buf);
        // use partial_send funtion to make sure we have sent all the data
        if (partial_send(sockfd, buf, &len) == -1) {
            perror("partial_send failed");
            printf("We only sent %d bytes because of the error!\n", len);
        } 

        printf("%s", buf);
        fflush(stdout);
        memset(&buf, 0, sizeof buf);   

    } while(read_flag > 0);

    // if we get out the while loop above, it means that we have read and sent everything
    // which means the client is successful, so we close the socket and return 0
    close(sockfd);
    return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv) {
  char *server_ip;
  char *server_port;

  if (argc != 3) {
    fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
    exit(EXIT_FAILURE);
  }

  server_ip = argv[1];
  server_port = argv[2];

  int port_num = atoi(server_port);
  // add error checking for ip and port number
  if ( port_num < 10000 || port_num > 60000) {
    fprintf(stderr, "port number is wrong\n");
    exit(EXIT_FAILURE);
  }

  if (strcmp(server_ip, "127.0.0.1")!=0){
    fprintf(stderr, "ip address is wrong\n");
    exit(EXIT_FAILURE);
  }

  return client(server_ip, server_port);
}
