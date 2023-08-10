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
#include "rooms.h"
#include "clients_file.h"

#define LENGTH_TYPE short
#define NAME_MAX_LENGTH sizeof(LENGTH_TYPE) * 8
#define DATA_MAX_LENGTH 1 << NAME_MAX_LENGTH

#define INVALID_ACTION "Invalid action"
#define INVALID_NAME "Invalid name"
#define WRONG_PASSWORD "Wrong password"
#define NO_CLIENT "The client with the given name doesn't exist"
#define NOT_CONNECTED "The client with the given name isn't connected"
#define CLIENT_CANT_JOIN "Client already in another room / not connected"

typedef enum
{
    REGISTER_REQUEST = 100,
    LOGIN_REQUEST,
    LIST_ROOMS_REQUEST,
    JOIN_ROOM_REQUEST,
    SEND_MESSAGE_IN_ROOM_REQUEST,
    EXIT_ROOM_REQUEST
} s_func_e;

void *handle_client_functions(void *arg);