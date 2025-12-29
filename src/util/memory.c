#include "memory.h"
#include "../status/status.h"
#include <stdlib.h>
#include <stdio.h>

#define CAP_PROGRESSION 2

void handleAllocError(const Status status)
{
    fprintf(stderr, "\n%s\n", status.message);
    exit(status.code);
}

void safeMalloc(void** ptr, const size_t size)
{
    void* tmp = malloc(size);
    if (tmp == NULL) handleAllocError(MALLOC_FAIL);
    *ptr = tmp;
}

void safeRealloc(void** ptr, const size_t size)
{
    void* tmp = realloc(*ptr, size);
    if (tmp == NULL) handleAllocError(REALLOC_FAIL);
    *ptr = tmp;
}

void dynamicGrowth(void** ptr, size_t* cap, const size_t elem_size)
{
    *cap *= CAP_PROGRESSION;
	safeRealloc(ptr, *cap * elem_size);
}

void free_array(void **array, const size_t count)
{
    size_t i;

    if (!array) return;

    i = 0;
    while (i < count)
        free(array[i++]);

    free(array);
}
