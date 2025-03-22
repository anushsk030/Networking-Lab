
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
// int n,sockfd,addr=sizeof(struct sockaddr_in);
// char buf[1024];
// struct sockaddr_in x;
// if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
// printf("error in creating socket \n");
// else
// printf("socket successfully created\n");
// x.sin_family=AF_INET;
// x.sin_port=htons(PORT);
// x.sin_addr.s_addr=htonl(INADDR_ANY);
// bind(sockfd,(struct sockaddr*)&x,sizeof(x));
// printf("waiting...\n");
// do
// {
// n=recvfrom(sockfd,buf,1024,0,(struct sockaddr*)&x,&addr);
// buf[n]='\0';
// if(n>1)
// printf("received : %s \n",buf);
// scanf("%s",buf);
// sendto(sockfd,buf,n,0,(struct sockaddr*)&x,addr);
// }
// while(strcmp(buf,"quit")!=0);
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

int main() {
    int sockfd, n;
    socklen_t len = sizeof(struct sockaddr_in);
    char buf[MAX];
    struct sockaddr_in servaddr = {0};

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

    // Convert and store IP address
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    // Message loop
    while (1) {
        // Get user input
        printf("Enter message: ");
        fgets(buf, MAX, stdin);
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&servaddr, len);

        // If message is "quit", terminate
        if (strncmp(buf, "quit", 4) == 0) {
            printf("Client exiting...\n");
            break;
        }

        // Receive response from server
        n = recvfrom(sockfd, buf, MAX, 0, (struct sockaddr*)&servaddr, &len);
        buf[n] = '\0';  // Null-terminate the received message

        printf("From Server: %s\n", buf);
    }

    // Close socket
    close(sockfd);
    return 0;
}
