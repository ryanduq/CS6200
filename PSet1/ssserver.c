#include <stdio.h>
#include <netdb.h>

int main(void)
{

    //create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;

    //set socket options


    //bind socket
    int bind(sockfd, &server, )


    //listen for client


    //accept client


    //read message from client


    //close server



    
    return 0;
}