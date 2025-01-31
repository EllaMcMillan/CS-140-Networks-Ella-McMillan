// Ella McMillan

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_IP "10.115.12.240"
#define SERVER_PORT 49153
#define MAX_MSG_LEN 1024 

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char username[50];
    char msg[MAX_MSG_LEN];

    // Create the Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Setup the server address struct
    server_addr.sin_family = AF_INET;
    //SERVER_IP
    server_addr.sin_port = htons(SERVER_PORT);

    int e_code = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    //check the e_code with error message
    if (e_code <= 0) {
        error("Invalid server IP address");
    }
    
    // //SERVER_IP
    // server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    // error logic
    if (fgets(username, sizeof(username), stdin) == NULL) {
        error("error reading username");
    }
    

    username[strcspn(username, "\n")] = 0; // removes the newline character
    
    // Send username to the server
    send(sockfd, username, strlen(username), 0);
    // Main loop to send/receive messages
    while (1) {
        
        // Read message from the user
        if (fgets(msg, sizeof(msg), stdin) == NULL) {
            // error logic
            error("Error reading input");
        }
        
        //if length of fgets is a blank line, don't send it, but go through the process of receiving new messages.
        if (strlen(msg) == 0) {
            goto receive;
        }
        

        // Remove newline from message and send to server
        //msg[strcspn(msg, "\n")] = 0;

        //send the message
        send(sockfd, msg, strlen(msg), 0);

        //somehow clear the message
        msg[0] = 0;
        receive:    
            //receive what's coming in on the socket
            int n = recv(sockfd, msg, sizeof(msg), 0);
            // need to check n with error code
            if (n == -1) {
                error("Error receiving message");
            } else if (n == 0) {
                printf("Server closed the connection.\n");
                exit(0);
            }

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