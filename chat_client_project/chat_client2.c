#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "10.115.12.240"
#define SERVER_PORT 49153
#define MAX_MSG_LEN 1024 //do i need this


int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char username[50];
    char msg[MAX_MSG_LEN];

    // Create the Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Setup the server address struct
    server_addr.sin_family = AF_INET;
    int e_code = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    //check the e_code with error message
    
    //SERVER_IP
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    //add error logic

    username[strcspn(username, "\n")] = 0; // removes the newline character
    
    // Send username to the server
    send(sockfd, username, strlen(username), 0);
    // Main loop to send/receive messages
    while (1) {
        
        // Read message from the user
        fgets(msg, sizeof(msg), stdin);
        //add error logic
        //if length of fgets is a blank line, don't send it, but go through the process of receiving new messages.

        // Remove newline from message and send to server
        //msg[strcspn(msg, "\n")] = 0;

        //send the message
        send(sockfd, msg, strlen(msg), 0);

        //somehow clear the message
        msg[0] = 0;
        //receive what's coming in on the socket
        int n = recv(sockfd, msg, sizeof(msg), 0);
        //check n with error code

        msg[n] = '\0'; // Null-terminate the received string
        // Display received message
        printf("%s\n", msg);
    }
    // Close socket
    close(sockfd);
    return 0;
}

//ideas
//get username from commandline
//