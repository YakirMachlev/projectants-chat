#pragma once

#include <pthread.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <sys/socket.h>

#define MAX_CLIENTS 50
#define GIVEN_NUM_OF_ROOMS 5
#define NUM_OF_ROOMS GIVEN_NUM_OF_ROOMS + 1
#define SERVER_FULL_MSG "Current server full"

typedef struct client_t client_t;
typedef struct room_t room_t;

uint8_t get_room_id(client_t *client);
void init_chat_rooms();
bool check_room_availability(uint8_t room_id);
void add_client_to_room(char *client_name, uint8_t room_id, int sockfd);
client_t *search_client_in_room(uint8_t room_id, int sockfd);
client_t *search_client(int sockfd);
void remove_client_from_room(uint8_t room_id, int sockfd);
void remove_client(int sockfd);
void send_rooms_list(int sockfd);
void send_message_to_room(uint8_t room_id, char *msg, int len);