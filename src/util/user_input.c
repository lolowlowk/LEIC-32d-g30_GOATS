#include "user_input.h"
#include "string_util.h"
#include "memory.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#define INITIAL_STR_CAP 4

#define BUFFER_CAP 1024
#define COUNTING_BASE 10

bool dealOverflow() 
{
    return errno != ERANGE;
}

Status processNaturalNum(const char* str, size_t* num) 
{
    size_t i, cap;

    char* strProcessing = NULL;

    i = 0;
    cap = INITIAL_STR_CAP;
    errno = 0;

    safeMalloc((void**) &strProcessing, cap);

    while (*str != '\0') 
    {
        if (isdigit(*str)) 
        {
            strProcessing[i++] = *(str++);
            if (i + 1 == cap)
                dynamicGrowth((void**)&strProcessing, &cap);
                
            continue;
        }

        if (!isspace(*(str++))) // Ignores white space and flags for repetition if character isnt a number nor whitespace
        {   
            free(strProcessing);
            return INVALID_NUMBER;
        }
    }

    if (i < 1)
    {   
        free(strProcessing);
        return INVALID_NUMBER;
    }

    strProcessing[i] = '\0';
    *num = strtoll(strProcessing, NULL, COUNTING_BASE);

    if (!dealOverflow()) 
        return NUMBER_OVERFLOW;

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
    
    free(trimmed);
}
