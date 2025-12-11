#include "table_filter.h"
#include "../util/memory.h"
#include "../status/status.h"
#include <string.h>
#include <stdbool.h>

// Helper: copy an entire row from src → dest
static void copy_row_to_table(table* dest, const row* src_row)
{
    char* src_cell;
    
    size_t dest_row_index;

    unsigned short col;

    table_add_row(dest);

    dest_row_index = dest->row_num - 1;

    for (col = 0; col < dest->col_num; col++)
    {
        src_cell = src_row->cells[col];
        table_set_cell(dest, dest_row_index, 'A' + col, src_cell);
    }
}

table* table_filter(const table* t, bool (*predicate)(const void* row, const void* context), const void* context)
{
    table* result = NULL;

    row* src_row = NULL;

    size_t r;

    if (t == NULL)
    {
        announceStatus(INVALID_TABLE);
        return NULL;
    }

    // Create result table
    result = table_create(t->col_num);
    if (result == NULL)
        return NULL;

    // Loop through rows of the original table
    for (r = 0; r < t->row_num; r++)
    {
        src_row = &(t->rows[r]);

        // Evaluate predicate
        if (predicate((const void*)src_row, context))
            // Row is accepted → copy to result table
            copy_row_to_table(result, src_row);
    }

    return result;
}
