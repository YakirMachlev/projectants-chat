#include "clients_file.h"

static pthread_mutex_t file_lock = PTHREAD_MUTEX_INITIALIZER;

bool check_name_validity(char *name)
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

void insert_client_to_file(char *name, char *password)
{
    FILE *fp;

    pthread_mutex_lock(&file_lock);
    fp = fopen("users.txt", "a");
    assert("Couldn't open the file" && fp);

    fputs(name, fp);
    fputc(',', fp);
    fputs(password, fp);
    fputc('\n', fp);

    fclose(fp);
    pthread_mutex_unlock(&file_lock);
}

static uint8_t compare_until_char(char* str1, char* str2, char ch)
{
    uint8_t offset;
    for (offset = 0; offset < 16 && str1[offset] != ch && str2[offset] != ch; offset++)
    {
        if (str1[offset] != str2[offset])
        {
            return false;
        }
    }
    return offset;
}

bool client_file_does_client_exist(char *name)
{
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    bool exists;    
    char *line_ptr;

    pthread_mutex_lock(&file_lock);
    fp = fopen("users.txt", "r");
    exists = false;
    if (fp)
    {
        while (fgets(line, MAX_LINE_LENGTH, fp) && !exists)
        {
            line_ptr = line;
            exists = compare_until_char(line, name, ',');
        }
        fclose(fp);        
    }
    pthread_mutex_unlock(&file_lock);

    return !!exists;
}

bool client_file_check_client_validity(char *name, char *password)
{
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    uint8_t length;
    char *line_ptr;
    bool valid;

    pthread_mutex_lock(&file_lock);
    fp = fopen("users.txt", "r");
    
    if (fp)
    {
        while (fgets(line, MAX_LINE_LENGTH, fp))
        {
            line_ptr = line;
            if ((valid = compare_until_char(line, name, ',')))
            {
                line_ptr += length;
                valid &= strncmp(password, line_ptr, PASSWORD_MAX_LENGTH);
                break;
            }
        }
        fclose(fp);        
    }
    pthread_mutex_unlock(&file_lock);
}