#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdbool.h>

// Open a file for reading
FILE* open_file(const char *fileName, const char* action);

// Read a single line (allocated dynamically)
void readLine(FILE* f, char** destination, bool* isOver);

#endif // FILE_UTILS_H
