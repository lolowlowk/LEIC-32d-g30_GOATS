#ifndef TABLE_H
#define TABLE_H

#include <stddef.h>
#include "../status/status.h"

#include <stddef.h>
#include <stdbool.h>

#define MAX_COLS 26

//////////////////////////////////

typedef struct {
    char** cells;
} row;

typedef struct {
    row* rows;
    size_t row_num;
    unsigned short col_num;
} table;

/////////////////////////////////

// Table functions
table* table_create(unsigned short cols);
void table_add_row(table* t);
Status table_set_cell(table* t, size_t row, char col_letter, const char* value);
char* table_get_cell_contents(const table* t, size_t row, char col_letter, Status* error_code);
Status table_remove_last_row(table* t);
void table_destroy(table** tptr);

// Utility
char** table_get_cell_ref(const table* t, const size_t row, const char col_letter, Status* status);
int col_letter_to_index(char col);

#endif
