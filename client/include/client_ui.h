#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "client_functions.h"

void client_ui_start(void* arg);
void first_hierarchy(int sockfd);


void client_ui_register_response(char *buffer);
