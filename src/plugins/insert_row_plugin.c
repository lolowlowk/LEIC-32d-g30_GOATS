#include <stdio.h>
#include <stdlib.h>
#include "../command_prompt/command.h"
#include "../table/table.h"
#include "insert_row_plugin.h"

// plugin that inserts a row into a table
static void cmd_insert_row(Context* ctx, const char** args)
{
    table* t = ctx->table;
    if (!t) { printf("No table loaded.\n"); return; }

    if (!args[0]) { printf("Usage: insert_row <row_index>\n"); return; }

    size_t row_index = atoi(args[0]);
    if (row_index > t->row_num) {
        printf("Row index out of bounds.\n");
        return;
    }

    table_insert_row(t, row_index);
    printf("Row inserted at index %zu.\n", row_index);
}

// plugin init function
void init_plugin(CommandRegistry* registry)
{
    register_command(registry, "insert_row", "Insert a new row at given index", cmd_insert_row, 1);
}
