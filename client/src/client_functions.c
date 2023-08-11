#include "client_functions.h"

static bool client_functions_check_name_validity(char *name)
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

static void client_functions_start(int sockfd, request_e request)
{
    char buffer[DATA_MAX_LENGTH];
    char name[NAME_MAX_LENGTH + 1];
    char password[PASSWORD_MAX_LENGTH + 1];
    uint8_t name_length;
    uint8_t password_length;
    uint8_t total_length;

    name[0] = '-';
    while (!client_functions_check_name_validity(name))
    {
        printf("%s", "Enter your name (only alpha-numeric characters):\n> ");
        fgets(name, NAME_MAX_LENGTH, stdin);
    }
    name_length = strlen(name);

    printf("%s", "Enter your password:\n> ");
    fgets(password, PASSWORD_MAX_LENGTH, stdin); 
    password_length = strlen(password);

    total_length = snprintf(buffer, CLIENT_DATA_LENGTH, "%d%d%s%d%s", request, name_length, name, password_length, password);
    send(sockfd, buffer, total_length, 0);
}

void client_functions_register(int sockfd)
{
    client_functions_start(sockfd, REGISTER_REQUEST);
}

void client_functions_login(int sockfd)
{
    client_functions_start(sockfd, LOGIN_REQUEST);
}

void client_functions_list_rooms(int sockfd)
{
    send(sockfd, LIST_ROOMS_REQUEST, 1, 0);
}