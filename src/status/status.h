#ifndef STATUS_H
#define STATUS_H

typedef struct {
    int code;
    const char* message;
} Status;

void announceStatus(Status status);

// Common error codes
extern const Status OK;
extern const Status MALLOC_FAIL;
extern const Status REALLOC_FAIL;
extern const Status INVALID_COLUMN;
extern const Status INVALID_TABLE;
extern const Status INVALID_ROW;
extern const Status INVALID_FILE_NAME;
extern const Status CANNOT_OPEN_FILE;

#endif // ERRORS_H
