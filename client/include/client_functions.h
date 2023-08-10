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
    LOGIN_SUCCESS,
    LIST_ROOMS,
    JOIN_ROOM_SUCCESS,
    SEND_MESSAGE_IN_ROOM,
    EXIT_ROOM_SUCCESS,
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