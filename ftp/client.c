#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX 1024  // Buffer size

void main() {
    int sockfd, port;
    char filename[MAX], new_filename[MAX], buffer[MAX];
    struct sockaddr_in serveraddr;
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
    printf("Socket successfully created.\n");

    // Initialize server address
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }
    printf("Connected to server.\n");

    // Get file details
    printf("Enter the existing file name: ");
    scanf("%s", filename);
    printf("Enter the new file name: ");
    scanf("%s", new_filename);

    // Open file for writing
    fp = fopen(new_filename, "w");
    if (fp == NULL) {
        perror("File creation failed");
        close(sockfd);
        exit(1);
    }

    // Send requested filename to server
    send(sockfd, filename, strlen(filename), 0);

    // Receive and write file contents
    while (1) {
        int n = recv(sockfd, buffer, MAX, 0);
        buffer[n] = '\0';  // Null-terminate received data

        if (strcmp(buffer, "error") == 0) {
            printf("File not found on server.\n");
            fclose(fp);
            close(sockfd);
            exit(1);
        }

        if (strcmp(buffer, "completed") == 0) {
            printf("\nFile transfer completed.\n");
            break;
        } else {
            fputs(buffer, fp);
            fputs(buffer, stdout);
        }
    }

    // Close file and socket
    fclose(fp);
    close(sockfd);

    
}

