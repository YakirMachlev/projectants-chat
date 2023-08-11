#include "rooms.h"

struct room_t
{
    uint8_t num_of_clients;
    client_t *clients[NUM_OF_CONNECTIONS];
    pthread_mutex_t lock;
};

static room_t *chat_rooms[NUM_OF_ROOMS];

void init_chat_rooms()
{
    int room_num;
    for (room_num = 0; room_num < NUM_OF_ROOMS; ++room_num)
    {
        chat_rooms[room_num]->num_of_clients = 0;
        pthread_mutex_init(&(chat_rooms[room_num]->lock), NULL);
    }
}

void add_client_to_room(client_t *client, uint8_t room_num)
{
    uint8_t room_num_of_clients;

    pthread_mutex_lock(&(chat_rooms[room_num]->lock));
    room_num_of_clients = chat_rooms[room_num]->num_of_clients++;
    chat_rooms[client->room_id]->clients[room_num_of_clients] = client;
    pthread_mutex_unlock(&(chat_rooms[client->room_id]->lock));
}

void remove_client_from_room(client_t *client)
{
    uint8_t client_num;
    room_t *room;

    room = chat_rooms[client->room_id];
    pthread_mutex_lock(&(room->lock));
    for (client_num = 0; client_num < room->num_of_clients; client_num++)
    {
        if (room->clients[client_num] == client)
        {
            room->clients[client_num] = room->clients[room->num_of_clients - 1];
            room->num_of_clients--;
            break;
        }
    }
    pthread_mutex_unlock(&(room->lock));
}

void get_rooms_list(client_t *client, uint8_t *rooms_list)
{
    uint8_t room_num;

    *(rooms_list++) = LIST_ROOMS_RESPONSE;
    *(rooms_list++) = NUM_OF_ROOMS;
    pthread_mutex_lock(&(chat_rooms[client->room_id]->lock));
    for (room_num = 0; room_num < NUM_OF_ROOMS; room_num++)
    {
        *(rooms_list++) = chat_rooms[room_num]->num_of_clients;
    }
    pthread_mutex_unlock(&(chat_rooms[client->room_id]->lock));
}

void send_message_to_room(uint8_t room_id, char *msg, int len)
{
    uint8_t client_num;
    room_t *room;

    room = chat_rooms[room_id];
    pthread_mutex_lock(&(room->lock));
    for (client_num = 0; client_num < room->num_of_clients; client_num++)
    {
        send(room->clients[client_num]->sockfd, msg, len, 0);
    }
    pthread_mutex_unlock(&(room->lock));
}