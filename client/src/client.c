#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "client_functions.h"
#include "client_ui.h"
#include "client_recv.h"

state_e client_state;

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
    pthread_t client_ui_thread;
    char input[DATA_MAX_LENGTH];
    int bytes_sent;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(SERVER_IP, SERVER_PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    sockfd = client_find_server_valid_address(servinfo);

    pthread_create(&receive_thread, NULL, receive_data_from_server, (void *)&sockfd);
    pthread_create(&client_ui_thread, NULL, client_ui_start, (void *)&sockfd);

    while (true)
    {
        bytes_sent = send(sockfd, input, strlen(input), 0);
        if (bytes_sent == -1)
        {
        }

        if (strcmp(input, "~`") == 0)
        {
            break;
        }
    }
    close(sockfd);

    return 0;
}
