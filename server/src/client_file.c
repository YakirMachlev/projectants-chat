#include "clients_file.h"

bool check_name_validity(char *name)
{
    uint8_t offset;
    for (offset = 0; offset < 16 && *name; offset++)
    {
        if (!isalnum(*(name++)))
        {
            return false;
        }
    }
    return true;
}

void insert_client_to_file(char *name, char *password)
{
    FILE *fp;

    fp = fopen("users.txt", "a");
    assert("Couldn't open the file" && fp);

    fputs(name, fp);
    fputc(',', fp);
    fputs(password, fp);
    fputc('\n', fp);

    fclose(fp);
}

static uint8_t compare_until_char(char* str1, char* str2, char ch)
{
    uint8_t offset;
    for (offset = 0; offset < 16 && str1[offset] != ch && str2[offset] != ch; offset++)
    {
        if (str1[offset] != str2[offset])
        {
            return 0;
        }
    }
    return offset;
}

void get_password_by_name(char *name, char* password)
{
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    uint8_t length;
    char *line_ptr;

    password = NULL;
    fp = fopen("users.txt", "r");
    assert("Couldn't open the file" && fp);
    while (fgets(line, MAX_LINE_LENGTH, fp))
    {
        line_ptr = line;
        if ((length = compare_until_char(line, name, ',')))
        {
            line_ptr += length;
            strncpy(password, line_ptr, 32);
            break;
        }
    }
    fclose(fp);
}