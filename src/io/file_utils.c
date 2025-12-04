#include "file_utils.h"
#include "../util/memory.h"

// Open input file
FILE* openInputFile(const char *fileName)
{
    if (fileName == NULL) 
    {
        fprintf(stderr, "Error: fileName is NULL\n");
        return NULL;
    }

    FILE *file = fopen(fileName, "r");
    if (file == NULL)
        fprintf(stderr, "Error: Could not open input file\n");
        
    return file;
}

void readLine(const FILE* f, char** destination, bool* isOver)
{
    char currentChar;

    size_t i, cap;

    cap = INITIAL_LINE_CAP;

    safeMalloc((void**) destination, cap);

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

// Read all lines dynamically from input
char** getAllLines(FILE *inputFile, size_t *numLines)
{
    size_t linesCap = INITIAL_LINES_CAP;
    bool EOFReached = false;
    char **lines = NULL;

    safeMalloc((void**)&lines, linesCap * sizeof(char*));

    *numLines = 0;
    while (!EOFReached)
    {
        if (*numLines >= linesCap)
            dynamicGrowth((void**)&lines, &linesCap, sizeof(char*));

        readLine(inputFile, &lines[*numLines], &EOFReached);

        if (lines[*numLines] != NULL)
            (*numLines)++;
    }

    return lines;
}

// Free all allocated lines
void freeLines(char **lines, const size_t numLines)
{
    for (size_t i = 0; i < numLines; i++)
        free(lines[i]);
    free(lines);
}