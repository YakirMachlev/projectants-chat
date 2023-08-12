#include "client_recv.h"

static void handle_recv_from_server(char *buffer, int length)
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
        client_ui_login_response(buffer);
        break;
    case LIST_ROOMS_RESPONSE:
        client_ui_list_rooms_response(buffer);
        break;
    case JOIN_ROOM_RESPONSE:
        client_ui_join_room_response(buffer);
        break;
    case SEND_MESSAGE_IN_ROOM_RESPONSE:
        client_ui_send_message_in_room_response(buffer);
        break;
    case EXIT_ROOM_RESPONSE:
        client_ui_exit_room_response(buffer);
        break;
    case SEND_SERVER_MESSAGE_IN_ROOM:
        printf("%s", buffer);
        break;
    default:
        printf("%s", "Invalid response.\n");
        break;
    }
}

void *receive_data_from_server(void *arg)
{
    int sockfd;
    char buffer[DATA_MAX_LENGTH];
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
        handle_recv_from_server(buffer, bytes_received);
    }

    return NULL;
}
