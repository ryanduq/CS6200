#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>

/* CONSTANTS =============================================================== */
#define SERVER_PORT 8888

int main(void)
{
    int sockfd, clientsockfd;

    int set_reuse_addr = 1; // ON == 1  
    int max_pending_connections = 1;

    struct sockaddr_in saddr, caddr;
    socklen_t client_addr_len; 

    //create socket
    if(0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        fprintf(stderr, "server failed to create the listening socket\n");
        exit(1);
    }
    else
    {
        printf("server created the listening socket\n");
    }

    // Set socket to use wildcards - i.e. 0.0.0.0:21 and 192.168.0.1:21
    // can be bound separately (helps to avoid conflicts) 
    if (0 != setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &set_reuse_addr, sizeof(set_reuse_addr))) {
        fprintf(stderr, "server failed to set SO_REUSEADDR socket option (not fatal)\n");
    }

    // Configure server socket address structure (init to zero, IPv4,
    // network byte order for port and address)
    // Address uses local wildcard 0.0.0.0.0 (will connect to any local addr)
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SERVER_PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);


    //bind socket
    if(0 > bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)))
    {
        fprintf(stderr, "server failed to bind\n");
        exit(1);
    }

    // Listen on the socket for up to some maximum pending connections
    if (0 > listen(sockfd, max_pending_connections)) {
        fprintf(stderr, "server failed to listen\n");
        exit(1);
    } else {
        fprintf(stdout, "server listening for a connection on port %d\n", SERVER_PORT);
    }

    // Get the size client's address structure
    client_addr_len = sizeof(caddr);

    // Accept a new client
    if (0 > (clientsockfd = accept(sockfd, (struct sockaddr *)&caddr, &client_addr_len))) 
    {
        fprintf(stderr, "server accept failed\n");
    } else {
        fprintf(stdout, "server accepted a client!\n");
        close(clientsockfd);
    }

    //read message from client


    //close server
    if(0 > close(sockfd))
    {
        printf("cannot close server socket\n");
    }
    else
    {
        printf("server socket closed\n");
    }



    
    return 0;
}