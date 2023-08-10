#include "server_functions.h"

/* int sendall(int s, char *buf, int *len)
{
    int total;
    int bytes_left;
    int bytes_sent;

    total = 0;
    bytes_left = *len;
    while (total < *len)
    {
        bytes_sent = send(s, buf + total, bytes_left, 0);
        if (bytes_sent == -1)
        {
            break;
        }
        total += bytes_sent;
        bytes_left -= bytes_sent;
    }
    *len = total;

    return bytes_sent == -1 ? -1 : 0;
} */

void client_register(int client_sockfd)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char password[32];

    if ((bytes_received = recv(client_sockfd, name, sizeof(name) / sizeof(char), 0)) == -1)
    {
        perror("name recv");
        close(client_sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((bytes_received = recv(client_sockfd, password, sizeof(password) / sizeof(char), 0)) == -1)
    {
        perror("password recv");
        close(client_sockfd);
        return;
    }
    password[bytes_received] = '\0';

    if (check_name_validity(name))
    {
        insert_client_to_file(name, password);
    }
    else
    {
        send(client_sockfd, INVALID_NAME, strlen(INVALID_NAME), 0);
    }
}

void client_login(int client_sockfd)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char password[32];
    char written_password[32];

    if ((bytes_received = recv(client_sockfd, name, sizeof(name) / sizeof(char), 0)) == -1)
    {
        perror("name recv");
        close(client_sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((bytes_received = recv(client_sockfd, password, sizeof(password) / sizeof(char), 0)) == -1)
    {
        perror("password recv");
        close(client_sockfd);
        return;
    }
    password[bytes_received] = '\0';

    get_password_by_name(name, written_password);
    if (written_password != NULL)
    {
        if (strncmp(password, written_password, 32) == 0)
        {
            add_client_to_room(name, NUM_OF_ROOMS - 1, client_sockfd);
        }
        else
        {
            send(client_sockfd, WRONG_PASSWORD, strlen(WRONG_PASSWORD), 0);
        }
    }
    else
    {
        send(client_sockfd, NO_CLIENT, strlen(NO_CLIENT), 0);
    }
}

void client_list_rooms(int client_sockfd)
{
    if (search_client(client_sockfd))
    {
        send_rooms_list(client_sockfd);
    }
    else
    {
        send(client_sockfd, NOT_CONNECTED, strlen(NOT_CONNECTED), 0);
    }
}

void client_join_room(int client_sockfd)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char room_str[2];
    uint8_t room_num;
    char connection_msg[NAME_MAX_LENGTH + 12];

    if ((bytes_received = recv(client_sockfd, name, sizeof(name) / sizeof(char), 0)) == -1)
    {
        perror("name recv");
        close(client_sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((bytes_received = recv(client_sockfd, room_str, sizeof(room_str) / sizeof(char), 0)) == -1)
    {
        perror("room_str recv");
        close(client_sockfd);
        return;
    }
    room_str[bytes_received] = '\0';

    if (search_client_in_room(NUM_OF_ROOMS - 1, client_sockfd))
    {
        room_num = atoi(room_str);
        snprintf(connection_msg, sizeof(connection_msg) / sizeof(char), "%s connected.", name);
        send_message_to_room(room_num, connection_msg, sizeof(connection_msg) / sizeof(char));
        add_client_to_room(name, room_num, client_sockfd);
    }
    else
    {
        send(client_sockfd, CLIENT_CANT_JOIN, strlen(CLIENT_CANT_JOIN), 0);
    }
}

void client_send_massage_in_room(int client_sockfd)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char msg[DATA_MAX_LENGTH];
    client_t *client;

    if ((bytes_received = recv(client_sockfd, name, sizeof(name) / sizeof(char), 0)) == -1)
    {
        perror("name recv");
        close(client_sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((bytes_received = recv(client_sockfd, msg, sizeof(msg) / sizeof(char), 0)) == -1)
    {
        perror("msg recv");
        close(client_sockfd);
        return;
    }
    msg[bytes_received] = '\0';


    if ((client = search_client(client_sockfd)))
    {
        send_message_to_room(get_room_id(client), msg, sizeof(msg) / sizeof(char));
    }
    else
    {
        send(client_sockfd, NOT_CONNECTED, strlen(NOT_CONNECTED), 0);
    }
}

void client_exit_room(int client_sockfd)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char disconnection_msg[NAME_MAX_LENGTH + 12];
    client_t *client;

    if ((bytes_received = recv(client_sockfd, name, sizeof(name) / sizeof(char), 0)) == -1)
    {
        perror("name recv");
        close(client_sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((client = search_client(client_sockfd)))
    {
        remove_client_from_room(get_room_id(client), client_sockfd);
        snprintf(disconnection_msg, sizeof(disconnection_msg) / sizeof(char), "%s connected.", name);
        send_message_to_room(get_room_id(client), disconnection_msg, sizeof(disconnection_msg) / sizeof(char));
    }
    else
    {
        send(client_sockfd, NOT_CONNECTED, strlen(NOT_CONNECTED), 0);
    }
}

void *handle_client_functions(void *arg)
{
    int client_sockfd;
    short action_num;
    char action[4];
    int bytes_received;

    client_sockfd = *(int *)arg;
    while (true)
    {
        if ((bytes_received = recv(client_sockfd, action, 4, 0)) == -1)
        {
            perror("recv");
            close(client_sockfd);
            return NULL;
        }
        else if (bytes_received == 0)
        {
            client_exit_room(client_sockfd);
            close(client_sockfd);
            return NULL;
        }
        else
        {
            action[bytes_received] = '\0';
            action_num = atoi(action);

            switch (action_num)
            {
            case REGISTER_REQUEST:
                client_register(client_sockfd);
                break;
            case LOGIN_REQUEST:
                client_login(client_sockfd);
                break;
            case LIST_ROOMS_REQUEST:
                client_list_rooms(client_sockfd);
                break;
            case JOIN_ROOM_REQUEST:
                client_join_room(client_sockfd);
                break;
            case SEND_MESSAGE_IN_ROOM_REQUEST:
                client_send_massage_in_room(client_sockfd);
                break;
            case EXIT_ROOM_REQUEST:
                client_exit_room(client_sockfd);
                break;
            default:
                send(client_sockfd, INVALID_ACTION, strlen(INVALID_ACTION), 0);
            }
        }
    }
}