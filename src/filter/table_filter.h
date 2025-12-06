#ifndef TABLE_FILTER_H
#define TABLE_FILTER_H

#include <stdbool.h>
#include "../table/table.h"

table* table_filter(const table* t, bool (*predicate)(const void *row, const void *context), const void* context);

#endif // TABLE_FILTER_H
