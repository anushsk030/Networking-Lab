#include <arpa/inet.h>   // For inet_pton(), sockaddr_in
#include <netinet/in.h>  // For struct sockaddr_in
#include <stdio.h>       // For standard I/O functions
#include <stdlib.h>      // For exit(), memory functions
#include <string.h>      // For memset(), string operations
#include <sys/socket.h>  // For socket(), bind(), listen(), accept()
#include <unistd.h>      // For read(), write(), close()

#define MAX 80          // Maximum buffer size for messages
#define PORT 8080       // Port number where the server listens
#define SA struct sockaddr  // Short form for sockaddr structure

// Function to handle communication with the client
void func(int connfd) {
    char buff[MAX];  // Buffer to store messages

    while (1) {  // Infinite loop for chat
        memset(buff, 0, sizeof(buff));  // Clear the buffer

        // Read message from the client
        read(connfd, buff, sizeof(buff));
        printf("From Client: %s", buff);

        // If the client sends "exit", terminate chat
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Server Exit...\n");
            break;
        }

        // Send response back to the client
        printf("Enter response: ");
        fgets(buff, MAX, stdin);  // Read input from the server user
        write(connfd, buff, strlen(buff));  // Send response to the client
    }
}

int main() {
    int sockfd, connfd;  // Socket file descriptors
    struct sockaddr_in servaddr = {0};  // Server address structure 
    socklen_t len = sizeof(struct sockaddr_in);  // Size of client address

    // Step 1: Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed"); 
        exit(1);
    }
    printf("Socket successfully created..\n");

    // Step 2: Initialize the server address structure
   // memset(&servaddr, 0, sizeof(servaddr));

    // Step 3: Assign IP address and port number
    servaddr.sin_family = AF_INET;  // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    servaddr.sin_port = htons(PORT);  // Convert port number to network byte order

    // Step 4: Bind the socket to the given IP and port
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Bind failed");
        exit(1);
    }
    printf("Server bound to port %d..\n", PORT);

    // Step 5: Start listening for incoming connections
    if (listen(sockfd, 5) != 0) {
        perror("Listen failed");
        exit(1);
    }
    printf("Server listening...\n");

    // Step 6: Accept a client connection
    connfd = accept(sockfd, NULL, &len);  // NULL since we don't use client info
    if (connfd < 0) {
        perror("Accept failed");
        exit(1);
    }
    printf("Client connected.\n");

    // Step 7: Start the chat function
    func(connfd);

    // Step 8: Close the socket after communication ends
    close(sockfd);

    return 0;
}
