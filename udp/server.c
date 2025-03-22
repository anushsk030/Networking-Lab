
// #include<stdio.h>
// #include<sys/socket.h>
// #include<sys/types.h>
// #include<netinet/in.h>
// #include<arpa/inet.h>
// #include<string.h>
// #include <unistd.h>
// #define PORT 4000
// int main()
// {
// int m,n=100,sockfd,addr=sizeof(struct sockaddr_in);
// char buf[1025];
// struct sockaddr_in y;
// sockfd=socket(AF_INET,SOCK_DGRAM,0);
// y.sin_family=AF_INET;
// y.sin_port=htons(PORT);
// y.sin_addr.s_addr=htonl(INADDR_ANY);
// printf("connecting....\n");
// while(1)
// {
// printf("msg to be send :");
// scanf("%s",buf);
// sendto(sockfd,buf,n,0,(struct sockaddr*)&y,addr);
// if(strcmp(buf,"quit")==0)
// break;
// printf("msg send \n waiting for the response\n");
// n=recvfrom(sockfd,buf,1024,0,(struct sockaddr*)&y,&addr);
// buf[n]='0';
// if(n>1)
// printf("\n received :%s \n",buf);
// }
// close(sockfd);
// return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 4000
#define MAX 1024
#define SA struct sockaddr
int main() {
    int sockfd, n;
    socklen_t len = sizeof(struct sockaddr_in);
    char buf[MAX];
    struct sockaddr_in servaddr = {0}, cliaddr;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("Socket successfully created\n");

    // Assign IP and PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    printf("Server bound to port %d\nWaiting for messages...\n", PORT);

    // Message loop
    while (1) {
        // Receive message from client
        n = recvfrom(sockfd, buf, MAX, 0, (struct sockaddr*)&cliaddr, &len);
        buf[n] = '\0';  // Null-terminate the received message

        printf("From Client: %s\n", buf);

        // If client sends "quit", terminate
        if (strcmp(buf, "quit") == 0) {
            printf("Server exiting...\n");
            break;
        }

        // Send response back to client
        printf("Enter response: ");
        fgets(buf, MAX, stdin);
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, len);
    }

    // Close socket
    close(sockfd);
    return 0;
}
 