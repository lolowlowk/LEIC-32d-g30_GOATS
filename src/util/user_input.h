#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <stddef.h>   // for size_t
#include "../status/status.h"

// Parses a natural number from a string.
// Accepts only digits and whitespace.
// Returns OK on success, ERROR on invalid input.
// Result is written to *num.
Status processNaturalNum(const char* str, size_t* num);

// Prompts the user and reads a trimmed line of input.
// Allocates memory for *input (caller must free).
void get_input(const char prompt[], char **input);

#endif // USER_INPUT_H
