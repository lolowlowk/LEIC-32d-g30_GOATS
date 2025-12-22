#include <stdio.h>
#include <stdlib.h>
#include "../command_prompt/command.h"
#include "../table/table.h"
#include "delete_row_plugin.h"

// Command that removes a row by index
static void cmd_delete_row(Context* ctx, const char** args)
{
    table* t = ctx->table;
    if (!t) { printf("No table loaded.\n"); return; }

    if (!args[0]) { printf("Usage: delete_row <row_index>\n"); return; }

    size_t row_index = atoi(args[0]);
    if (row_index >= t->row_num) {
        printf("Row index out of bounds.\n");
        return;
    }

    table_delete_row(t, row_index);
    printf("Row %zu deleted.\n", row_index);
}


// plugin initialization
void init_plugin(CommandRegistry* registry)
{
    register_command(registry, "delete_row", "Remove a row by index", cmd_delete_row, 1);
}
