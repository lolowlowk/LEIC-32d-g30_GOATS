#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "string_util.h"
#include "../util/memory.h"

#define INITIAL_TOKENS_CAP 4

// Helper function to allocate and store a token
void add_token(char*** tokens, size_t* num_tokens, size_t* cap, const char* start, const size_t len)
{
    char* token;
    safeMalloc((void**)&token, len + 1);
    memcpy(token, start, len);
    token[len] = '\0';

    if (*num_tokens >= *cap)
        dynamicGrowth((void**)tokens, (size_t*) cap, sizeof(char*));   

    (*tokens)[(*num_tokens)++] = token;
}

// Generic split function
char** split(const char* str, const char sep, size_t* count) 
{
    size_t num_tokens = 0, tokens_cap = INITIAL_TOKENS_CAP;
    char** tokens;
    const char *start = str, *ptr = str;

    safeMalloc((void**)&tokens, tokens_cap * sizeof(char*));

    while (*ptr != '\0') 
    {
        if (*ptr == sep) 
        {
            add_token(&tokens, &num_tokens, &tokens_cap, start, ptr - start);
            while (*(++ptr) == sep);          // skip separator
            start = ptr;
            continue;
        }
        ptr++;
    }

    // Add last token
    if (ptr != start)
        add_token(&tokens, &num_tokens, &tokens_cap, start, ptr - start);
    
    *count = num_tokens;
    return tokens;
}

char* trim(const char* str)
{
    const char *start = str;
    while (isspace(*start)) start++;

    const char *end = start + strlen(start);
    while (end > start && isspace(end[-1])) end--;

    size_t len = end - start;

    char *out = NULL;
    safeMalloc((void **)& out, len + 1);
    memcpy(out, start, len);
    out[len] = '\0';

    return out;
}
