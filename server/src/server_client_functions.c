#include "server_client_functions.h"

void client_register(client_t *client, char *buffer)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char password[PASSWORD_MAX_LENGTH];

    if ((bytes_received = recv(client->sockfd, name, NAME_MAX_LENGTH, 0)) == -1)
    {
        perror("name recv");
        close(client->sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((bytes_received = recv(client->sockfd, password, PASSWORD_MAX_LENGTH, 0)) == -1)
    {
        perror("password recv");
        close(client->sockfd);
        return;
    }
    password[bytes_received] = '\0';

    if (check_name_validity(name))
    {
        if (1)
        {
            insert_client_to_file(name, password);
        }
    }
    else
    {
        send(client->sockfd, -1, 2, 0);
    }
}

void client_login(client_t *client, char *buffer)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char password[PASSWORD_MAX_LENGTH];
    char written_password[PASSWORD_MAX_LENGTH];

    if ((bytes_received = recv(client->sockfd, name, NAME_MAX_LENGTH, 0)) == -1)
    {
        perror("name recv");
        close(client->sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((bytes_received = recv(client->sockfd, password, PASSWORD_MAX_LENGTH, 0)) == -1)
    {
        perror("password recv");
        close(client->sockfd);
        return;
    }
    password[bytes_received] = '\0';

    get_password_by_name(name, written_password);
    if (written_password != NULL)
    {
        if (strncmp(password, written_password, PASSWORD_MAX_LENGTH) == 0)
        {
            add_client_to_room(name, NUM_OF_ROOMS - 1, client->sockfd);
        }
        else
        {
            send(client->sockfd, -1, 2, 0);
        }
    }
    else
    {
        send(client->sockfd, -1, 2, 0);
    }
}

void client_list_rooms(client_t *client, char *buffer)
{
    if (search_client(client->sockfd))
    {
        send_rooms_list(client->sockfd);
    }
    else
    {
        send(client->sockfd, -1, 2, 0);
    }
}

void client_join_room(client_t *client, char *buffer)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char room_str[2];
    uint8_t room_num;
    char connection_msg[NAME_MAX_LENGTH + 12];

    if ((bytes_received = recv(client->sockfd, name, NAME_MAX_LENGTH, 0)) == -1)
    {
        perror("name recv");
        close(client->sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((bytes_received = recv(client->sockfd, room_str, sizeof(room_str) / sizeof(char), 0)) == -1)
    {
        perror("room_str recv");
        close(client->sockfd);
        return;
    }
    room_str[bytes_received] = '\0';

    if (search_client_in_room(NUM_OF_ROOMS - 1, client->sockfd))
    {
        room_num = atoi(room_str);
        snprintf(connection_msg, sizeof(connection_msg) / sizeof(char), "%s connected.", name);
        send_message_to_room(room_num, connection_msg, sizeof(connection_msg) / sizeof(char));
        add_client_to_room(name, room_num, client->sockfd);
    }
    else
    {
        send(client->sockfd, -1, 2, 0);
    }
}

void client_send_massage_in_room(client_t *client, char *buffer)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char msg[DATA_MAX_LENGTH];
    client_t *client;

    if ((bytes_received = recv(client->sockfd, name, NAME_MAX_LENGTH, 0)) == -1)
    {
        perror("name recv");
        close(client->sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((bytes_received = recv(client->sockfd, msg, DATA_MAX_LENGTH, 0)) == -1)
    {
        perror("msg recv");
        close(client->sockfd);
        return;
    }
    msg[bytes_received] = '\0';


    if ((client = search_client(client->sockfd)))
    {
        send_message_to_room(get_room_id(client), msg, sizeof(msg) / sizeof(char));
    }
    else
    {
        send(client->sockfd, -1, 2, 0);
    }
}

void client_exit_room(client_t *client, char *buffer)
{
    int bytes_received;
    char name[NAME_MAX_LENGTH];
    char disconnection_msg[NAME_MAX_LENGTH + 12];
    client_t *client;

    if ((bytes_received = recv(client->sockfd, name, sizeof(name) / sizeof(char), 0)) == -1)
    {
        perror("name recv");
        close(client->sockfd);
        return;
    }
    name[bytes_received] = '\0';
    if ((client = search_client(client->sockfd)))
    {
        remove_client_from_room(get_room_id(client), client->sockfd);
        snprintf(disconnection_msg, sizeof(disconnection_msg) / sizeof(char), "%s connected.", name);
        send_message_to_room(get_room_id(client), disconnection_msg, sizeof(disconnection_msg) / sizeof(char));
    }
    else
    {
        send(client->sockfd, -1, 2, 0);
    }
}