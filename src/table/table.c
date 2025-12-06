#include "table.h"
#include "../util/memory.h"
#include "../status/status.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


table* table_create(unsigned const short cols) 
{
    table* pNew_table = NULL;

    if (cols <= 0 || cols > MAX_COLS) 
    {
        fprintf(stderr, "\nInvalid number of columns: %hu (allowed: 1-%d)\n", cols, MAX_COLS);
        return NULL;
    }

    safeMalloc((void**) &pNew_table, sizeof(table));

    pNew_table->rows = NULL;
    pNew_table->row_num = 0;
    pNew_table->col_num = cols;

    return pNew_table;
}

void table_add_row(table* t)
{
    int cell;

    if (t == NULL)
    {
        fprintf(stderr, "\nInvalid table\n");
        return;
    }

    safeRealloc((void**) &(t->rows), (t->row_num + 1) * sizeof(row)); // Reallocate rows address to have an address with +1 row spot available

    safeMalloc((void**) &(t->rows[t->row_num].cells), t->col_num * sizeof(char*)); // Allocate new row's cells to have col_num spots for strings available

    for (int cell = 0; cell < t->col_num; cell++)
        t->rows[t->row_num].cells[cell] = NULL;  

    t->row_num++;
}

int col_letter_to_index(const char col) 
{
    if (col >= 'A' && col <= 'Z') return col - 'A';
    if (col >= 'a' && col <= 'z') return col - 'a';
    return INVALID_COLUMN.code;
}

Status table_set_cell(table* t, const size_t row, const char col_letter, const char* value)
{   
    Status status;
    
    status = OK;
    char** cell = table_get_cell_ref(t, row, col_letter, &status);

    if (cell == NULL) 
        return status;

    free(*cell); // Free previous contents

    if (value == NULL) 
    {
        *cell = NULL;
        return status;
    }

    safeMalloc((void**) cell, strlen(value) + 1);
    strcpy(*cell, value);

    return status;
}

char* table_get_cell_contents(const table* t, const size_t row_num, const char col_letter, Status* status)
{
    char** cell = table_get_cell_ref(t, row_num, col_letter, status);

    if (cell == NULL)
        return NULL;

    return *cell; // may be NULL
}


char** table_get_cell_ref(const table* t, const size_t row, const char col_letter, Status* status) 
{
    if (t == NULL) 
    {   
        *status = INVALID_TABLE;
        return NULL;
    }

    if (row >= t->row_num) 
    {
        *status = INVALID_ROW;
        return NULL;
    }

    int col = col_letter_to_index(col_letter);
    if (col < 0) 
    {
        *status = INVALID_COLUMN;
        return NULL;
    }

    return &(t->rows[row].cells[col]);
}

Status table_remove_last_row(table* t)
{
    int cell;

    if (t == NULL || t->row_num == 0)
        return INVALID_ROW;

    t->row_num--;

    for (int cell = 0; cell < t->col_num; cell++) // Free each cell
        free(t->rows[t->row_num].cells[cell]);

    free(t->rows[t->row_num].cells); // Free row

    // Shrink rows array
    if (t->row_num != 0) 
    {
        safeRealloc((void**)&t->rows, t->row_num * sizeof(row));
        return OK;
    }
        
    free(t->rows);  // Free manually instead of reallocing to 0 because safeRealloc deals with reallocing to 0 as an error
    t->rows = NULL;

    return OK;
}

void table_destroy(table** tptr)
{
    table* t;

    if (tptr == NULL || *tptr == NULL)
        return;

    t = *tptr;

    // remove all rows safely
    while (t->row_num > 0)
        table_remove_last_row(t);

    free(t);
    *tptr = NULL;
}

