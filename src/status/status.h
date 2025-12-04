#ifndef STATUS_H
#define STATUS_H

typedef struct {
    int code;
    const char* message;
} Status;

// Common error codes
static const Status OK                  = {0, "OK"};
static const Status MALLOC_FAIL         = {1, "Memory allocation failed"};
static const Status REALLOC_FAIL        = {2, "Memory reallocation failed"};
static const Status INVALID_COLUMN      = {3, "Invalid column"};
static const Status INVALID_TABLE       = {4, "Invalid table pointer"};
static const Status INVALID_ROW         = {5, "Invalid row"};
static const Status CANNOT_OPEN_FILE    = {6, "Unable to open file"};

#endif // ERRORS_H
