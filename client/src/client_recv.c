#include "client_recv.h"

static void handle_recv_from_server(int sockfd, char *buffer, int length)
{
    response_e response;
    uint8_t result;

    response = *(buffer++);

    switch (response)
    {
    case REGISTER_RESPONSE:
        client_ui_register_response(buffer);
        break;
    case LOGIN_RESPONSE:
        break;
    case LIST_ROOMS_RESPONSE:
        break;
    case JOIN_ROOM_RESPONSE:
        break;
    case SEND_MESSAGE_IN_ROOM_RESPONSE:
        break;
    case EXIT_ROOM_RESPONSE:
        break;
    case SEND_SERVER_MESSAGE_IN_ROOM:
        break;
    default:
        break;
    }
}

void *receive_data_from_server(void *arg)
{
    int sockfd;
    char buffer[DATA_MAX_LENGTH + 1];
    int bytes_received;

    sockfd = *((int *)arg);
    while (true)
    {
        bytes_received = recv(sockfd, buffer, DATA_MAX_LENGTH, 0);
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
        handle_recv_from_server(sockfd, buffer, bytes_received);
    }

    return NULL;
}
