#include "rooms.h"

struct client_t
{
    int sockfd;
    uint8_t room_id;
    char name[16];
};

struct room_t
{
    uint8_t num_of_clients;
    client_t *clients[MAX_CLIENTS];
    pthread_mutex_t lock;
};

room_t *chat_rooms[NUM_OF_ROOMS];
pthread_mutex_t room_lock;
int connected_clients;

uint8_t get_room_id(client_t *client)
{
    return client->room_id;
}

void init_chat_rooms()
{
    int room_num;
    pthread_mutex_init(&room_lock, NULL);
    connected_clients = 0;
    for (room_num = 0; room_num < NUM_OF_ROOMS; ++room_num)
    {
        chat_rooms[room_num]->num_of_clients = 0;
        pthread_mutex_init(&(chat_rooms[room_num]->lock), NULL);
    }
}

bool check_room_availability(uint8_t room_id)
{
    bool available;
    pthread_mutex_lock(&room_lock);
    available = chat_rooms[room_id]->num_of_clients < MAX_CLIENTS;
    pthread_mutex_unlock(&room_lock);

    return available;
}

void add_client_to_room(char *client_name, uint8_t room_id, int sockfd)
{
    client_t *client;
    uint8_t room_num_of_clients;

    pthread_mutex_lock(&(chat_rooms[room_id]->lock));
    room_num_of_clients = chat_rooms[room_id]->num_of_clients++;
    client = chat_rooms[room_id]->clients[room_num_of_clients];
    strncpy(client->name, client_name, 16);
    client->room_id = room_id;
    client->sockfd = sockfd;
    pthread_mutex_unlock(&(chat_rooms[room_id]->lock));
}

client_t *search_client_in_room(uint8_t room_id, int sockfd)
{
    uint8_t client_num;
    room_t *room;
    client_t *client;

    client = NULL;
    room = chat_rooms[room_id];
    pthread_mutex_lock(&(room->lock));
    for (client_num = 0; client_num < room->num_of_clients; client_num++)
    {
        if (room->clients[client_num]->sockfd == sockfd)
        {
            client = room->clients[client_num];
            break;
        }
    }
    pthread_mutex_unlock(&(room->lock));

    return client;
}

client_t *search_client(int sockfd)
{
    uint8_t room_num;
    client_t *found_client;

    for (room_num = 0; room_num < NUM_OF_ROOMS && !found_client; room_num++)
    {
        found_client = search_client_in_room(room_num, sockfd);
    }

    return found_client;
}

void remove_client_from_room(uint8_t room_id, int sockfd)
{
    uint8_t client_num;
    room_t *room;

    room = chat_rooms[room_id];
    pthread_mutex_lock(&(room->lock));
    for (client_num = 0; client_num < room->num_of_clients; client_num++)
    {
        if (room->clients[client_num]->sockfd == sockfd)
        {
            room->clients[client_num] = room->clients[room->num_of_clients - 1];
            room->num_of_clients--;
            break;
        }
    }
    pthread_mutex_unlock(&(room->lock));
}

void remove_client(int sockfd)
{
    uint8_t room_num;
    for (room_num = 0; room_num < NUM_OF_ROOMS; room_num++)
    {
        remove_client_from_room(room_num, sockfd);
    }
}

void send_rooms_list(int sockfd)
{
    char line[4];
    uint8_t room_num;

    for (room_num = 0; room_num < GIVEN_NUM_OF_ROOMS; room_num++)
    {
        line[0] = room_num + 1 - '0';
        line[1] = ' ';
        line[2] = (chat_rooms[room_num]->num_of_clients / 10) - '0';
        line[3] = (chat_rooms[room_num]->num_of_clients % 10) - '0';
    }
    send(sockfd, line, 4, 0);
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