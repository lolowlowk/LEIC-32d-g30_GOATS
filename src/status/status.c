#include "status.h"
#include <stdio.h>

const Status OK                  = { "OK", 0 };
const Status MALLOC_FAIL         = { "Memory allocation failed", 1 };
const Status REALLOC_FAIL        = { "Memory reallocation failed", 2 };
const Status INVALID_COLUMN      = { "Invalid column", 3 };
const Status INVALID_TABLE       = { "Invalid table pointer", 4 };
const Status INVALID_ROW         = { "Invalid row", 5 };
const Status INVALID_FILE_NAME   = { "Invalid file name", 6 };
const Status CANNOT_OPEN_FILE    = { "Unable to open file", 7 };
const Status ERROR               = { "General error", 8 };

void announceStatus(Status status) 
{
    fprintf(stderr, "\n%s\n", status.message);
}