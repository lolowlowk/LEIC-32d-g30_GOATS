#include "status.h"
#include <stdio.h>

const Status OK                  = {0, "OK"};
const Status MALLOC_FAIL         = {1, "Memory allocation failed"};
const Status REALLOC_FAIL        = {2, "Memory reallocation failed"};
const Status INVALID_COLUMN      = {3, "Invalid column"};
const Status INVALID_TABLE       = {4, "Invalid table pointer"};
const Status INVALID_ROW         = {5, "Invalid row"};
const Status INVALID_FILE_NAME   = {6, "Invalid file name"};
const Status CANNOT_OPEN_FILE    = {7, "Unable to open file"};

void announceStatus(Status status) 
{
    fprintf(stderr, "\n%s\n", status.message);
}