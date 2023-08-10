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
#include "server_conf.h"

void client_register(client_t *client, char *buffer);
void client_login(client_t *client, char *buffer);
void client_list_rooms(client_t *client);
void client_join_room(client_t *client, char *buffer);
void client_send_massage_in_room(client_t *client, char *buffer);
void client_exit_room(client_t *client);