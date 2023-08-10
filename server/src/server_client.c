#include "server_client.h"

#define INIT_CLIENT(client, fd) \
    client->sockfd = fd;        \
    client->room_id = -1;       \
    client->state = EXISTS;

struct client_t
{
    int sockfd;
    uint8_t room_id;
    char name[NAME_MAX_LENGTH];
    state_e state;
};

void *server_client_handle_functions(void *arg)
{
    client_t *client;
    int client_sockfd;
    uint8_t opcode;
    char buffer[DATA_MAX_LENGTH];
    char *buffer_ptr;
    int bytes_received;

    client_sockfd = *(int *)arg;
    INIT_CLIENT(client, client_sockfd)

    while (true)
    {
        if ((bytes_received = recv(client_sockfd, buffer, DATA_MAX_LENGTH, 0)) <= 0)
        {
            if (bytes_received == -1)
            {
                perror("recv");
            }
            client_exit_room(client, buffer_ptr);
            close(client_sockfd);
            return NULL;
        }
        else
        {
            buffer_ptr = buffer;
            buffer[bytes_received] = '\0';
            opcode = *(buffer_ptr++);

            switch (opcode)
            {
            case REGISTER_REQUEST:
                client_register(client, buffer_ptr);
                break;
            case LOGIN_REQUEST:
                client_login(client, buffer_ptr);
                break;
            case LIST_ROOMS_REQUEST:
                client_list_rooms(client, buffer_ptr);
                break;
            case JOIN_ROOM_REQUEST:
                client_join_room(client, buffer_ptr);
                break;
            case SEND_MESSAGE_IN_ROOM_REQUEST:
                client_send_massage_in_room(client, buffer_ptr);
                break;
            case EXIT_ROOM_REQUEST:
                client_exit_room(client, buffer_ptr);
                break;
            default:
                client_exit_room(client, buffer_ptr);
                close(client_sockfd);
                return NULL;
            }
        }
    }
}