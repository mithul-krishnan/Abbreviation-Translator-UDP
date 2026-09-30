
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX 4096

int main(void)
{
    int sockfd;
    char message[MAX];
    char buffer[MAX];
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    printf("UDP Abbreviation Translator Client\n");
    printf("Enter a sentence (or type exit to quit):\n");

    while (1) {
        printf("\nYou: ");

        if (fgets(message, sizeof(message), stdin) == NULL)
            break;

        message[strcspn(message, "\n")] = '\0';

        if (strlen(message) == 0)
            continue;

        if (strcmp(message, "exit") == 0)
            break;

        if (sendto(sockfd, message, strlen(message) + 1, 0,
                   (struct sockaddr *)&server_addr,
                   server_len) < 0) {
            perror("Send failed");
            continue;
        }

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
                       &timeout, sizeof(timeout)) < 0) {
            perror("Timeout configuration failed");
            close(sockfd);
            return 1;
        }

        memset(buffer, 0, sizeof(buffer));

        ssize_t received = recvfrom(sockfd, buffer, MAX - 1, 0,
                                    (struct sockaddr *)&server_addr,
                                    &server_len);

        if (received < 0) {
            perror("Receive failed or server timed out");
            continue;
        }

        buffer[received] = '\0';
        printf("Translated: %s\n", buffer);
    }

    close(sockfd);
    printf("Client closed.\n");

    return 0;
}
