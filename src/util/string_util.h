#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <stddef.h>  // for size_t

void add_token(char*** tokens, size_t* num_tokens, size_t* cap, const char* start, size_t len);

// Splits a string by a given separator character.
// Returns a NULL-terminated array of strings.
// The number of tokens is written to *count.
// Caller must free each token and the token array itself.
char** split(const char* str, char sep, size_t* count);

// Trims leading and trailing whitespace from a string in-place.
// Returns a pointer to the trimmed string (which may be shifted inside the buffer).
char* trim(const char* str);

#endif // STRING_UTIL_H
