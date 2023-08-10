#pragma once

#include <inttypes.h>

#define LENGTH_TYPE uint16_t
#define NAME_MAX_LENGTH sizeof(LENGTH_TYPE) * 8
#define PASSWORD_MAX_LENGTH NAME_MAX_LENGTH * 2
#define DATA_MAX_LENGTH 1 << NAME_MAX_LENGTH

typedef enum
{
    EXISTS,
    CONNECTED,
    JOINED
} state_e;

struct client_t
{
    int sockfd;
    uint8_t room_id;
    char name[NAME_MAX_LENGTH];
    state_e state;
};

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
    REGISTER_SUCCESS = 200,
    LOGIN_SUCCESS,
    LIST_ROOMS_SUCCESS,
    JOIN_ROOM_SUCCESS,
    SEND_MESSAGE_IN_ROOM,
    EXIT_ROOM_SUCCESS,
    SEND_SERVER_MESSAGE_IN_ROOM = 211
} c_func_e;