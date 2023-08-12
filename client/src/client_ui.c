#include "client_ui.h"

client_t *client;

/* requests ui */

void client_ui_start(void* arg)
{
    int sockfd;

    sockfd = *(int *)arg;
    client->sockfd = sockfd;
    client->state = NONE;
    client_ui_first_hierarchy(sockfd);
}

void client_ui_first_hierarchy(int sockfd)
{
    uint8_t option;
    printf("%s", "choose 1 - registration / 2 - login");
    scanf("%d", &option);

    if (option == 1)
    {
        client_requests_register(sockfd);

    }
    else if (option == 2)
    {
        client_requests_login(sockfd);
    }
    else
    {
        printf("%s", "Invalid option.\n");
        client_ui_first_hierarchy(sockfd);
    }
}

void client_ui_second_hierarchy(int sockfd)
{
    uint8_t room_num;
    char buffer[DATA_MAX_LENGTH];
    uint16_t buffer_length;

    client_requests_list_rooms(sockfd);
    /* wait for response from server */
    printf("%s", "Choose which room you want to join\n");
    scanf("%d", &room_num);

    client->room_id = room_num;
    client_requests_join_room(sockfd);

    *buffer = '-';
    while (strncmp(buffer, "~`", 3))
    {
        scanf("%s", buffer);
        buffer_length = strlen(buffer);
        client_requests_send_message_in_room(sockfd, buffer, buffer_length);
    }
}


/* responses ui */

void client_ui_register_response(char *buffer)
{
    uint8_t result;

    result = *buffer;
    client->state = EXISTS;
    if (result == 0)
    {
        printf("%s", "Registration succeded\n");
        client_ui_first_hierarchy(client->sockfd);
    }
    else if (result == -1)
    {
        printf("%s", "Registration failed\n");
        client_ui_first_hierarchy(client->sockfd);
    }
    else
    {
        printf("%s", "Invalid response\n");
        client->state = NONE;
    }
}

void client_ui_login_response(char *buffer)
{
    uint8_t result;

    result = *buffer;
    if (result == 0)
    {
        printf("%s", "Login succeded\n");
        client->state = CONNECTED;
        client_ui_second_hierarchy(client->sockfd);
    }
    else if (result == -1)
    {
        printf("%s", "Wrong login details\n");
        client_ui_first_hierarchy(client->sockfd);
    }
    else
    {
        printf("%s", "Invalid response\n");
    }
}

void client_ui_list_rooms_response(char *buffer)
{
    uint8_t result;
    uint8_t offset;

    result = *buffer;
    if (result == -1)
    {
        printf("%s", "List rooms failed\n");
    }
    else
    {
        for (offset = 1; offset <= result; offset++)
        {
            printf("room #%d: %d\n", offset, *(++buffer));
        }
    }
}

void client_ui_join_room_response(char *buffer)
{
    uint8_t result;

    result = *buffer;
    if (result == 0)
    {
        printf("%s", "Join succeded\n");
        client->state = JOINED;
    }
    else if (result == -1)
    {
        printf("%s", "Join failed\n");
    }
    else
    {
        printf("%s", "Invalid response\n");
    }
}

void client_ui_send_message_in_room_response(char *buffer)
{
    uint8_t result;
    char *name;
    char *msg;
    uint8_t name_length;
    uint16_t msg_length;

    name_length = *(buffer++);
    name = buffer;

    buffer += name_length;
    msg_length = *(buffer++) << 8;
    msg_length |= *(buffer++);
    msg = buffer;

    name[name_length] = '\0';
    msg[msg_length] = '\0';

    printf("%s: %s", name, msg);
}

void client_ui_exit_room_response(char *buffer)
{
    uint8_t result;

    result = *buffer;
    if (result == 0)
    {
        printf("%s", "Exit room succeded\n");
        client->state = CONNECTED;
        client_ui_second_hierarchy(client->sockfd);
    }
    else if (result == -1)
    {
        printf("%s", "Exit room failed\n");
    }
    else
    {
        printf("%s", "Invalid response\n");
    }
}