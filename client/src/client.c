#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "client_functions.h"

#define MAX_MESSAGE_LEN 256
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "1234"

void *receive_messages(void *arg)
{
    int sockfd;
    char buffer[MAX_MESSAGE_LEN];
    int bytes_received;

    sockfd = *((int *)arg);
    while (true)
    {
        bytes_received = recv(sockfd, buffer, MAX_MESSAGE_LEN - 1, 0);
        if (bytes_received <= 0)
        {
            if (bytes_received == 0)
            {
                printf("server: socket %d hung up\n", sockfd);
            }
            else
            {
                perror("recv");
            }
            close(sockfd);
            break;
        }
        buffer[bytes_received] = '\0';
    }

    return NULL;
}

int client_find_server_valid_address(struct addrinfo *servinfo)
{
    int sockfd;
    struct addrinfo *p;

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);
    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        exit(1);
    }

    return sockfd;
}

int main()
{
    int sockfd;
    int rv;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    pthread_t receive_thread;
    char input[MAX_MESSAGE_LEN];
    int bytes_sent;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(SERVER_IP, SERVER_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    sockfd = client_find_server_valid_address(servinfo);

    pthread_create(&receive_thread, NULL, receive_messages, (void *)&sockfd);

    while (true)
    {

        bytes_sent = send(sockfd, input, strlen(input), 0);
        if (bytes_sent == -1)
        {
            // Handle send error
            // ...
        }

        // Check for disconnection request
        if (strcmp(input, "~`") == 0)
        {
            // Handle disconnection request
            // ...
            break;
        }
    }
    close(sockfd);

    return 0;
}
