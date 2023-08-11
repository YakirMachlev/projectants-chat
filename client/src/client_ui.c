#include "client_ui.h"


void client_ui_start(void* arg)
{
    int sockfd;
    state_e client_state;

    sockfd = *(int *)arg;
    while (true)
    {
        switch (client_state)
        {
        case EXISTS:
            first_hierarchy(sockfd, &client_state);
            break;
        case CONNECTED:
            second_hierarchy(sockfd);
            break;
        case JOINED:
            break;
        default:
            first_hierarchy(sockfd, &client_state); 
            break;
        }
        

    }
    second_hierarchy(sockfd);
}

void first_hierarchy(int sockfd, state_e *state)
{
    uint8_t option;
    int bytes_received;
    char buffer[DATA_MAX_LENGTH];
    printf("%s", "choose 1 - registration / 2 - login");
    scanf("%d", &option);

    if (option == 1)
    {
        client_functions_register(sockfd);

    }
    else if (option == 2)
    {
        client_functions_login(sockfd);
    }
    else
    {
        printf("%s", "Invalid option.\n");
        first_hierarchy(sockfd, state);
    }
}

void second_hierarchy(int sockfd)
{
    

}

void client_ui_register_response(char *buffer)
{
    uint8_t result;

    result = *buffer;
    if (result == 0)
    {
        printf("%s", "Registration succeded\n");
    }
    else if (result == -1)
    {
        printf("%s", "Registration failed\n");
    }
    else
    {
        printf("%s", "Registration help!!!\n");
    }
}

void client_ui_login_response(char *buffer)
{
    uint8_t result;

    result = *buffer;
    if (result == 0)
    {
        printf("%s", "Login succeded\n");
    }
    else if (result == -1)
    {
        printf("%s", "Login failed\n");
    }
    else
    {
        printf("%s", "Login help!!!\n");
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
