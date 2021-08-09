#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

/* CONSTANTS =============================================================== */
#define SERVER_ADDR "localhost"
#define SERVER_PORT 8888

int main(void)
{
    int sockfd;
    struct sockaddr_in saddr;

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

    // Converts localhost into 0.0.0.0
    struct hostent *he = gethostbyname(SERVER_ADDR); 
    unsigned long server_addr_nbo = *(unsigned long *)(he->h_addr_list[0]);

    // Configure server socket address structure (init to zero, IPv4,
    // network byte order for port and address) 
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SERVER_PORT);
    saddr.sin_addr.s_addr = server_addr_nbo;

    //connect to server
    if(0 > (sockfd = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr))))
    {
        fprintf(stderr, "client failed to connect to %s:%d!\n", SERVER_ADDR, SERVER_PORT);
        close(sockfd);
        exit(1);
    }
    else 
    {
        fprintf(stdout, "client connected to to %s:%d!\n", SERVER_ADDR, SERVER_PORT);
    }


    //send message to server


    //read message from server


    //close socket
    if(0 > close(sockfd))
    {
        printf("cannot close client socket\n");
    }
    else
    {
        printf("client socket closed\n");
    }
    



    return 0;
}