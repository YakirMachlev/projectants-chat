#include "server_client_functions.h"

#define CLIENT_DISCONNECT     \
    client_exit_room(client); \
    close(client->sockfd);    \
    pthread_exit(NULL);

#define ASSERT(expression) \
    if (expression)        \
    {                      \
        CLIENT_DISCONNECT  \
    }

#define CLIENT_FAILED(client, error, opcode) \
    error[0] = opcode; \
    error[1] = -1; \
    send(client->sockfd, error, ERROR_LENGTH, 0);

void client_register(client_t *client, char *buffer)
{
    char *password;
    uint8_t error[ERROR_LENGTH];
    uint8_t name_length;
    uint8_t password_length;

    name_length = *(buffer++);
    ASSERT(name_length <= NAME_MAX_LENGTH)
    strncpy(client->name, buffer, name_length);

    buffer += name_length;
    password_length = *(buffer++);
    ASSERT(password_length <= PASSWORD_MAX_LENGTH)
    password = buffer;
    password[password_length] = '\0';

    if (client->state = EXISTS && check_name_validity(client->name))
    {
        if (!client_file_does_client_exist(client->name))
        {
            insert_client_to_file(client->name, password);
        }
        else
        {
            CLIENT_FAILED(client, error, REGISTER_RESPONSE);
        }
    }
    else
    {
        CLIENT_FAILED(client, error, REGISTER_RESPONSE);
    }
}

void client_login(client_t *client, char *buffer)
{
    char *name;
    char *password;
    char written_password[PASSWORD_MAX_LENGTH];
    uint8_t error[ERROR_LENGTH];
    uint8_t name_length;
    uint8_t password_length;

    name_length = *(buffer++);
    ASSERT(name_length <= NAME_MAX_LENGTH)
    name = buffer;

    buffer += name_length;
    password_length = *(buffer++);
    ASSERT(password_length <= PASSWORD_MAX_LENGTH)
    password = buffer;

    name[name_length] = '\0';
    password[password_length] = '\0';

    if (client->state = EXISTS && check_name_validity(name))
    {
        if (client_file_check_client_validity(name, password))
        {
            client->state = CONNECTED;
        }
        else
        {
            CLIENT_FAILED(client, error, LOGIN_RESPONSE);
        }    
    }
    else
    {
        CLIENT_FAILED(client, error, LOGIN_RESPONSE);
    }
}

void client_list_rooms(client_t *client)
{
    uint8_t rooms_list[(NUM_OF_ROOMS << 1) + 2];
    uint8_t error[ERROR_LENGTH];

    if (client->state == CONNECTED)
    {
        get_rooms_list(client, rooms_list);
        send(client->sockfd, rooms_list, sizeof(rooms_list) / sizeof(uint8_t), 0);
    }
    else
    {
        CLIENT_FAILED(client, error, LIST_ROOMS_RESPONSE);
    }
}

void client_join_room(client_t *client, char *buffer)
{
    uint8_t room_num;
    char connection_msg[NAME_MAX_LENGTH + 12];
    uint8_t error[ERROR_LENGTH];
    uint8_t name_length;

    name_length = *(buffer++);
    ASSERT(name_length <= NAME_MAX_LENGTH)
    buffer += name_length;
    room_num = *buffer;

    if (client->state = CONNECTED)
    {
        snprintf(connection_msg, sizeof(connection_msg) / sizeof(char), "%s connected.", client->name);
        send_message_to_room(room_num, connection_msg, sizeof(connection_msg) / sizeof(char));
        add_client_to_room(client, room_num);
        client->state = JOINED;
        client->room_id = room_num;
    }
    else
    {
        CLIENT_FAILED(client, error, JOIN_ROOM_RESPONSE);
    }
}

void client_send_massage_in_room(client_t *client, char *buffer)
{
    char *msg;
    uint8_t error[ERROR_LENGTH];
    uint8_t name_length;
    uint16_t msg_length;

    name_length = *(buffer++);
    ASSERT(name_length <= NAME_MAX_LENGTH)
    buffer += name_length;
    msg_length = *buffer << 8 | *(++buffer);
    ASSERT(msg_length <= DATA_MAX_LENGTH)
    msg = buffer + 1;
    msg[msg_length] = '\0';

    if (client->state == JOINED)
    {
        send_message_to_room(client->room_id, msg, msg_length);
    }
    else
    {
        CLIENT_FAILED(client, error, SEND_MESSAGE_IN_ROOM_RESPONSE);
    }
}

void client_exit_room(client_t *client)
{
    uint8_t error[ERROR_LENGTH];
    char disconnection_msg[NAME_MAX_LENGTH + 15];

    if (client->state == JOINED)
    {
        remove_client_from_room(client);
        snprintf(disconnection_msg, sizeof(disconnection_msg) / sizeof(char), "%s disconnected.", client->name);
        send_message_to_room(client->room_id, disconnection_msg, sizeof(disconnection_msg) / sizeof(char));
        client->state = CONNECTED;
        client->room_id = -1;
    }
    else
    {
        CLIENT_FAILED(client, error, EXIT_ROOM_RESPONSE);
    }
}