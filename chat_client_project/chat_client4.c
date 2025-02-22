// Ella McMillan
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>

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
    char input[MAX_MSG_LEN] = "";
    int input_len = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        error("Socket creation failed");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) < 0) {
        error("Invalid server IP address");
    }
    
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Connection to server failed");
    }

    printf("Enter your username: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        error("Error reading username");
    }

    if (send(sockfd, username, strlen(username), 0) < 0) {
        error("Error sending username");
    }

    fd_set read_fds;
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(sockfd, &read_fds);

        if (select(sockfd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            error("Select error");
        }

        if (FD_ISSET(sockfd, &read_fds)) {
            int n = recv(sockfd, msg, sizeof(msg) - 1, 0); 
            if (n <= 0) {
                error("Server closed the connection.\n");
            }
            msg[n] = '\0';
            // printf("\r%*s\r", input_len, "");  // Clear input line
            printf("%s", msg); // Print server message
            // printf("%s> %s", username, input); // Reprint user input
            fflush(stdout);
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char ch;
            if (read(STDIN_FILENO, &ch, 1) > 0) {
                if (ch == '\n') {
                    strcat(input, "\n");
                    int n = send(sockfd, input, strlen(input), 0); 
                    if (n < 0) {
                        error("Error sending message");
                    }
                    input[0] = '\0';
                    // input = "";
                } else if (ch == 127 || ch == 8) { // Handle backspace
                    if (input_len > 0) {
                        input[strlen(input) - 1] = '\0';
                        printf("\b \b");
                    }
                } else {
                    input[input_len++] = ch;
                    input[input_len] = '\0';
                    putchar(ch);
                }
                fflush(stdout);
            }
        }
    }
    close(sockfd);
    return 0;
}
