#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "string_util.h"
#include "../util/memory.h"

#define INITIAL_TOKENS_CAP 4

// Helper function to allocate and store a token
void add_token(char*** tokens, size_t* num_tokens, size_t* cap, const char* start, size_t len)
{
    char* token;
    safeMalloc((void**)&token, len + 1);
    strncpy(token, start, len);
    token[len] = '\0';

    if (*num_tokens >= *cap)
        dynamicGrowth((void**)tokens, (size_t*) cap);

    (*tokens)[(*num_tokens)++] = token;
}

// Generic split function
char** split(const char* str, char sep, size_t* count) {
    size_t num_tokens = 0, tokens_cap = INITIAL_TOKENS_CAP;
    char** tokens;
    const char *start = str, *ptr = str;

    safeMalloc((void**)&tokens, tokens_cap * sizeof(char*));

    while (*ptr != '\0') {
        if (*ptr == sep) {
            add_token(&tokens, &num_tokens, &tokens_cap, start, ptr - start);
            while (*(++ptr) == sep);          // skip separator
            start = ptr;
            continue;
        }
        ptr++;
    }

    // Add last token
    if (ptr != start) {
        add_token(&tokens, &num_tokens, &tokens_cap, start, ptr - start);
    }

    *count = num_tokens;
    return tokens;
}

char* trim(char* str)
{
    int i, j;
    char* newStr = NULL;
    
    i = 0;
    j = strlen(str) - 1;

    while (isspace(str[i])) i++;

    if (str[i] == '\0') 
    {
        str[0] = '\0';
        return str;
    }

    newStr = str + i;

    while (isspace(str[j])) j--;

    str[j+1] = '\0';

    return newStr;
}