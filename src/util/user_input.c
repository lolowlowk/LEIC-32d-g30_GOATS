#include "../status/status.h"
#include "user_input.h"
#include "string_util.h"
#include "memory.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define INITIAL_STR_CAP 4

#define BUFFER_CAP 999

Status processNaturalNum(const char* str, size_t* num) 
{
    size_t i, cap;

    char* strProcessing = NULL;

    i = 0;
    cap = INITIAL_STR_CAP;

    safeMalloc((void**) &strProcessing, cap);

    while (*str != '\0') 
    {
        if (isdigit(*str)) 
        {
            strProcessing[i++] = *(str++);
            if (i == cap)
                dynamicGrowth((void**) &strProcessing, &cap);
                
            continue;
        }

        if (!isspace(*(str++))) // Ignores white space and flags for repetition if character isnt a number nor whitespace
        {   
            free(strProcessing);
            return ERROR;
        }
    }

    if (i < 1)
    {   
        free(strProcessing);
        return ERROR;
    }

    strProcessing[i] = '\0';
    *num = atoll(strProcessing);

    free(strProcessing);
    
    return OK;
}

void get_input(const char prompt[], char **input) 
{
    char buf[BUFFER_CAP]; // fixed-size buffer for fgets
    char *trimmed;

    size_t len;

    printf("%s", prompt);

    if (!fgets(buf, sizeof(buf), stdin)) {
        *input = NULL;
        return; // error or EOF
    }

    trimmed = trim(buf);
    len = strlen(trimmed);

    safeMalloc((void**) input, len + 1); // +1 for '\0'

    strcpy(*input, trimmed);
}
