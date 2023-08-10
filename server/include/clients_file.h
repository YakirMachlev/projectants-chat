#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH 50

bool check_name_validity(char *name);
void insert_client_to_file(char *name, char *password);
void get_password_by_name(char *name, char *password);