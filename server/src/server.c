#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "server_functions.h"

#define PORT "1234"
#define NUM_OF_CONNECTIONS 51

void *handle_new_connections(void *arg)
{
    int server_sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    int client_sockfd;
    pthread_t client_thread;

    server_sockfd = *(int *)arg;
    client_addr_len = sizeof(client_addr);
    while (true)
    {
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sockfd == -1)
        {
            perror("accept");
            continue;
        }
        pthread_create(&client_thread, NULL, handle_client_functions, (void *)(&client_sockfd));
        pthread_detach(client_thread);
    }
    return NULL;
}

int server_find_valid_address(struct addrinfo *servinfo)
{
    int server_socket;
    struct addrinfo *p;
    int yes = 1;

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }
        if (bind(server_socket, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(server_socket);
            perror("server: bind");
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);
    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    return server_socket;
}

int main()
{
    int server_sockfd;
    pthread_t connection_thread;
    int rv;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    init_chat_rooms();

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "server: %s\n", gai_strerror(rv));
        return 1;
    }
    server_sockfd = server_find_valid_address(servinfo);

    if (listen(server_sockfd, NUM_OF_CONNECTIONS) == -1)
    {
        perror("listen");
        exit(1);
    }

    printf("Waiting for connections\n");
    pthread_create(&connection_thread, NULL, handle_new_connections, (void *)(&server_sockfd));

    return 0;
}
