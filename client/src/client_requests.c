#include "client_requests.h"

static bool client_requests_check_name_validity(char *name)
{
    uint8_t offset;
    for (offset = 0; offset < NAME_MAX_LENGTH && *name; offset++)
    {
        if (!isalnum(*(name++)))
        {
            return false;
        }
    }
    return true;
} 

static void client_requests_start(int sockfd, request_e request)
{
    char buffer[CLIENT_DATA_LENGTH];
    char password[PASSWORD_MAX_LENGTH];
    uint8_t password_length;
    uint8_t total_length;

    client->name[0] = '-';
    while (!client_requests_check_name_validity(client->name))
    {
        printf("%s", "Enter your name (only alpha-numeric characters):\n> ");
        fgets(client->name, NAME_MAX_LENGTH, stdin);
    }
    client->name_length = strlen(client->name);

    printf("%s", "Enter your password:\n> ");
    fgets(password, PASSWORD_MAX_LENGTH, stdin); 
    password_length = strlen(password);

    total_length = snprintf(buffer, CLIENT_DATA_LENGTH, "%d%d%s%d%s", request, client->name_length, client->name, password_length, password);
    send(sockfd, buffer, total_length, 0);
}

void client_requests_register(int sockfd)
{
    client_requests_start(sockfd, REGISTER_REQUEST);
}

void client_requests_login(int sockfd)
{
    client_requests_start(sockfd, LOGIN_REQUEST);
}

void client_requests_list_rooms(int sockfd)
{
    send(sockfd, LIST_ROOMS_REQUEST, 1, 0);
}

void client_requests_join_room(int sockfd)
{
    char buffer[NAME_MAX_LENGTH + 3];

    buffer[0] = JOIN_ROOM_REQUEST;
    buffer[1] = 
    strncpy(buffer + 2, client->name, NAME_MAX_LENGTH);
    buffer[client->name_length + 2] = client->room_id;
    send(sockfd, buffer, sizeof(buffer) / sizeof(char), 0);
}