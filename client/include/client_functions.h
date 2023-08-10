#pragma once

#define __USE_XOPEN2K
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum
{
    REGISTER_SUCCESS = 200,
    LOGIN_RESPONSE,
    LIST_ROOMS_RESPONSE,
    JOIN_ROOM_RESPONSE,
    SEND_MESSAGE_IN_ROOM_RESPONSE,
    EXIT_ROOM_RESPONSE,
    SEND_SERVER_MESSAGE_IN_ROOM = 211
} c_func_e;

typedef enum
{
    REGISTER_REQUEST = 100,
    LOGIN_REQUEST,
    LIST_ROOMS_REQUEST,
    JOIN_ROOM_REQUEST,
    SEND_MESSAGE_IN_ROOM_REQUEST,
    EXIT_ROOM_REQUEST
} s_func_e;

#define INVALID_ACTION "Invalid action"
#define INVALID_NAME "Invalid name"
#define WRONG_PASSWORD "Wrong password"
#define NO_CLIENT "The client with the given name doesn't exist"
#define NOT_CONNECTED "The client with the given name isn't connected"
#define CLIENT_CANT_JOIN "Client already in another room / not connected"