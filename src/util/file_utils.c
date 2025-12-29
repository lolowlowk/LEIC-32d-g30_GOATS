#include "file_utils.h"
#include "memory.h"
#include "../status/status.h"

#include <stdlib.h>

#define INITIAL_LINE_CAP 80

// Open file
FILE* open_file(const char *fileName, const char* action)
{
    if (fileName == NULL) 
    {
        announceStatus(INVALID_FILE_NAME);
        return NULL;
    }

    FILE *file = fopen(fileName, action);
    if (file == NULL)
        announceStatus(CANNOT_OPEN_FILE);
        
    return file;
}

void readLine(FILE* f, char** destination, bool* isOver)
{
    char currentChar;

    size_t i, cap;

    cap = INITIAL_LINE_CAP;

    safeMalloc((void**) destination, cap * sizeof(char));

    i = 0;
    while (true)
    {
        currentChar = fgetc(f);

        if (currentChar == EOF) 
        {
            *isOver = true;
            if (i == 0) {
                free(*destination);    
                *destination = NULL;
                return;
            }
            
            (*destination)[i] = '\0';
            return;
        }

        if (currentChar == '\n')
        {
            (*destination)[i] = '\0';
            *isOver = false;
            return;
        }

        if (i >= cap)
            dynamicGrowth((void**) destination, &cap, sizeof(char));

        (*destination)[i++] = currentChar;
    }
}