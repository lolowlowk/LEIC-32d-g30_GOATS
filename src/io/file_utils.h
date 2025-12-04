#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// === Configurations ===
#define INITIAL_LINE_CAP 80
#define INITIAL_LINES_CAP 2


// Open a file for reading
FILE* openInputFile(const char *fileName);

// Read a single line (allocated dynamically)
void readLine(const FILE* f, char** destination, bool* isOver);

// Read all lines from a file (returns array of strings)
char** getAllLines(FILE *inputFile, size_t *numLines);

void freeLines(char **lines, const size_t numLines);

#endif // FILE_UTILS_H
