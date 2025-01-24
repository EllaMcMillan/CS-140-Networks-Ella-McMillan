#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "vmwardrobe.westmont.edu"
#define SERVER_PORT 49153
#define MAX_MSG_LEN 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    // char msg_size[MAX_MSG_LEN];
    char username[50];

    // Create the Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error creating socket");
    }

    // Setup the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        error("Invalid address or Address not supported");
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Connection failed");
    }

    // Send username to the server
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0; // removes the newline character

    // Send username to the server followed by a newline
    if (send(sockfd, username, strlen(username), 0) < 0) {
        error("Error sending username");
    }

    // Main loop to send/recieve messages
    while (1) {
        
        // Read message from the user
        printf("You: ");
        fgets(msg, sizeof(msg), stdin);
        
        // Remove newline from message and send to server
        msg[strcspn(msg, "\n")] = 0;
        if (send(sockfd, msg, strlen(msg), 0) < 0) {
            error("Error sending message");
        }

        // Recieve and display messages from server
        int n = recv(sockfd, msg, sizeof(msg)-1, 0);
        if (n <= 0) {
            printf("Server disconnected\n");
            break;
        }
        msg[n] = '\0'; // Null-terminate the recieved string
        // Display recieved message
        printf("Server: %s\n", msg);
    }
    // Close socket
    close(sockfd);
    return 0;
}

