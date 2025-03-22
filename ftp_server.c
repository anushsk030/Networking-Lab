#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX 1024  // Buffer size

int main() {
    int sockfd, clientfd, port, bind_status;
    char filename[MAX], filedata[MAX];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    FILE *fp;

    // Get port number
    printf("Enter the port number: ");
    scanf("%d", &port);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("Socket created successfully.\n");

    // Initialize server address structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // Bind the socket
    bind_status = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (bind_status < 0) {
        perror("Bind failed");
        exit(1);
    }
    printf("Bind successful.\n");

    // Listen for incoming connections
    listen(sockfd, 5);
    printf("Waiting for a connection...\n");

    // Accept client connection
    clientfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
    if (clientfd < 0) {
        perror("Accept failed");
        exit(1);
    }
    printf("Client connected.\n");

    // Receive filename from client
    int n = recv(clientfd, filename, sizeof(filename) - 1, 0);
    if (n < 0) {
        perror("Filename reception failed");
        close(clientfd);
        exit(1);
    }
    filename[n] = '\0';  // Null-terminate received filename

    // Open requested file
    fp = fopen(filename, "r");
    if (fp == NULL) {
        send(clientfd, "error", strlen("error"), 0);
        perror("File not found");
        close(clientfd);
        exit(1);
    }

    // Send file contents to client
    while (fgets(filedata, sizeof(filedata), fp)) {
        send(clientfd, filedata, strlen(filedata), 0);
        usleep(100000);  // Sleep to avoid flooding client
    }

    // Send completion message
    send(clientfd, "completed", strlen("completed"), 0);

    // Close file and socket
    fclose(fp);
    close(clientfd);
    close(sockfd);

    printf("File transfer completed. Server exiting.\n");
    return 0;
}


