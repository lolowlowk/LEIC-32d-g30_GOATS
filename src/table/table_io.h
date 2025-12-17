#ifndef TABLE_IO_H
#define TABLE_IO_H

#include "table.h"

// Loads a CSV file into a newly allocated table.
// Returns: pointer to table, or NULL if the file cannot be opened.
table* table_load_csv(const char* filename);

// Saves the table contents into a CSV file.
// If the table is NULL, announceStatus(INVALID_TABLE) is called.
void table_save_csv(const table* t, const char* filename);

#endif // TABLE_IO_H
