#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* CONSTANTS =============================================================== */
#define SERVER_ADDR "localhost"
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024
#define MESSAGE "ramblin wreck"

int main(int argc, char **argv)
{
    int sockfd = 0;
    struct sockaddr_in saddr;
    char buffer[BUFFER_SIZE];

    // Converts localhost into 0.0.0.0
    struct hostent *he = gethostbyname(SERVER_ADDR); 
    unsigned long server_addr_nbo = *(unsigned long *)(he->h_addr_list[0]);

    //create socket
    if(0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        fprintf(stderr, "failed to create the client socket\n");
        exit(1);
    }
    else
    {
        printf("created the client socket\n");
    }


    // Configure server socket address structure (init to zero, IPv4,
    // network byte order for port and address) 
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SERVER_PORT);
    saddr.sin_addr.s_addr = server_addr_nbo;

    //connect to server
    if(0 > connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr)) )
    {
        fprintf(stderr, "client failed to connect to %s:%d!\n", SERVER_ADDR, SERVER_PORT);
        close(sockfd);
        exit(1);
    }
    else 
    {
        fprintf(stdout, "client connected to %s:%d!\n", SERVER_ADDR, SERVER_PORT);
    }

    // Send echo message
    if (0 > send(sockfd, MESSAGE, strlen(MESSAGE), 0)) {
        fprintf(stderr, "client failed to send echo message");
        close(sockfd);
        exit(1);
    }

    //read message from server and print to terminal
    bzero(buffer, BUFFER_SIZE);
    if(0 > read(sockfd, buffer, BUFFER_SIZE)) {
        fprintf(stderr, "client could not read response from server\n");
        close(sockfd);
        exit(1);
    } else {
        fprintf(stdout, "echo from server: %s\n", buffer);
    }

    // Close the socket and return the response length (in bytes)
    close(sockfd);
    return 0;
}