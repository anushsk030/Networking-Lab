// #include <arpa/inet.h> // inet_addr()
// #include <netdb.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <strings.h> // bzero()
// #include <sys/socket.h>
// #include <unistd.h> // read(), write(), close()
// #define MAX 80
// #define PORT 8080
// #define SA struct sockaddr
// void func(int sockfd)
// {
// 	char buff[MAX];
// 	int n;
// 	for (;;) {
// 		bzero(buff, sizeof(buff));
// 		printf("Enter the string : ");
// 		n = 0;
// 		while ((buff[n++] = getchar()) != '\n')
// 			;
// 		write(sockfd, buff, sizeof(buff));
// 		bzero(buff, sizeof(buff));
// 		read(sockfd, buff, sizeof(buff));
// 		printf("From Server : %s", buff);
// 		if ((strncmp(buff, "exit", 4)) == 0) {
// 			printf("Client Exit...\n");
// 			break;
// 		}
// 	}
// }

// int main()
// {
// 	int sockfd, connfd;
// 	struct sockaddr_in servaddr, cli;

// 	// socket create and verification
// 	sockfd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (sockfd == -1) {
// 		printf("socket creation failed...\n");
// 		exit(0);
// 	}
// 	else
// 		printf("Socket successfully created..\n");
// 	bzero(&servaddr, sizeof(servaddr));

// 	// assign IP, PORT
// 	servaddr.sin_family = AF_INET;
// 	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
// 	servaddr.sin_port = htons(PORT);

// 	// connect the client socket to server socket
// 	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
// 		!= 0) {
// 		printf("connection with the server failed...\n");
// 		exit(0);
// 	}
// 	else
// 		printf("connected to the server..\n");

// 	// function for chat
// 	func(sockfd);

// 	// close the socket
// 	close(sockfd);
// }
#include <arpa/inet.h>  // For inet_pton(), sockaddr_in
#include <netinet/in.h> // For struct sockaddr_in
#include <stdio.h>      // For standard I/O functions
#include <stdlib.h>     // For exit(), memory functions
#include <string.h>     // For memset(), string operations
#include <sys/socket.h> // For socket(), connect()
#include <unistd.h>     // For read(), write(), close()

#define MAX 80         // Maximum buffer size for messages 
#define PORT 8080      // Port number where the server is listening

void func(int sockfd) {
    char buff[MAX];

    while (1) {
        memset(buff, 0, sizeof(buff));  // Clear buffer

        // Get user input
        printf("Enter message: ");
        fgets(buff, MAX, stdin);

        write(sockfd, buff, strlen(buff));  // Send message to server

        memset(buff, 0, sizeof(buff));  // Clear buffer
        read(sockfd, buff, sizeof(buff));  // Receive response from server

        printf("From Server: %s", buff);

        if (strncmp(buff, "exit", 4) == 0) {  // Exit if "exit" is received
            printf("Client exiting...\n");
            break;
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr = {0};  // Zero-initialize struct

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("Socket successfully created.\n");

    // Assign IP and PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    // // Convert and store IP address
    // if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
    //     perror("Invalid address");
    //     exit(1);
    // }

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection failed");
        exit(1);
    }
    printf("Connected to the server.\n");

    // Start chat function
    func(sockfd);

    // Close the socket
    close(sockfd);
    return 0;
}
