#pragma once

#include <inttypes.h>

#define PORT "1234"
#define NUM_OF_CONNECTIONS 50
#define SERVER_FULL_MSG "Current server full"
#define NAME_MAX_LENGTH 12
#define PASSWORD_MAX_LENGTH 32
#define DATA_MAX_LENGTH 1 << (sizeof(uint16_t) * 8)
#define ERROR_LENGTH 2


#define CLIENT_DISCONNECT     \
    client_exit_room(client); \
    close(client->sockfd);    \
    free(client);             \
    connected_clients--;      \
    pthread_exit(NULL);

#define ASSERT(expression) \
    if (expression)        \
    {                      \
        CLIENT_DISCONNECT  \
    }

typedef enum
{
    EXISTS,
    CONNECTED,
    JOINED
} state_e;

typedef struct
{
    int sockfd;
    uint8_t room_id;
    char name[NAME_MAX_LENGTH];
    state_e state;
} client_t;

typedef enum
{
    REGISTER_REQUEST = 100,
    LOGIN_REQUEST,
    LIST_ROOMS_REQUEST,
    JOIN_ROOM_REQUEST,
    SEND_MESSAGE_IN_ROOM_REQUEST,
    EXIT_ROOM_REQUEST
} s_func_e;

typedef enum
{
    REGISTER_RESPONSE = 200,
    LOGIN_RESPONSE,
    LIST_ROOMS_RESPONSE,
    JOIN_ROOM_RESPONSE,
    SEND_MESSAGE_IN_ROOM_RESPONSE,
    EXIT_ROOM_RESPONSE,
    SEND_SERVER_MESSAGE_IN_ROOM = 211
} c_func_e;