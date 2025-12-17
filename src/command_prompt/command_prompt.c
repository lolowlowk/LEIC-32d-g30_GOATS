#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dlfcn.h>

#include "command.h"     
#include "../table/table.h"
#include "../table/table_filter.h"
#include "../table/table_io.h"
#include "../util/memory.h"
#include "../util/string_util.h"
#include "../util/user_input.h"

#define DEFAULT_COMMANDS_AMOUNT 6

static void cmd_help(Context*, const char**);
static void cmd_exit(Context*, const char**);
static void cmd_load(Context*, const char**);
static void cmd_save(Context*, const char**);
static void cmd_show(Context*, const char**);
static void cmd_filter(Context*, const char**);

// All builtin commands
static const Command builtin_commands[DEFAULT_COMMANDS_AMOUNT] = {
    { "help",   "List available commands",          cmd_help,       0 },
    { "exit",   "Exit program",                     cmd_exit,       0 },
    
    { "load",   "Load table from file",             cmd_load,       1 },
    { "save",   "Save table to file",               cmd_save,       1 },
    { "show",   "Show a region of the table",       cmd_show,       1 },
    { "filter", "Filter rows based on column data", cmd_filter,     2 }
};

//----------------------------------------
//  Command Registry
//----------------------------------------
CommandRegistry* create_command_registry()
{
    size_t i;

    CommandRegistry* pNew_command_registry = NULL;

    safeMalloc((void**) &pNew_command_registry, sizeof(CommandRegistry));

    pNew_command_registry->commands = NULL;
    
    safeMalloc((void**) &(pNew_command_registry->commands), sizeof(Command) * DEFAULT_COMMANDS_AMOUNT);

    for (i = 0; i < DEFAULT_COMMANDS_AMOUNT; i++)
        pNew_command_registry->commands[i] = builtin_commands[i];

    pNew_command_registry->command_amount = DEFAULT_COMMANDS_AMOUNT;

    return pNew_command_registry;
}

void register_command(CommandRegistry* reg, const char* name, const char* description, const CommandFunc func)
{
    // grow the array if necessary
    size_t new_amount;

    new_amount = reg->command_amount + 1;
    safeRealloc((void**) &(reg->commands), sizeof(Command) * new_amount);

    reg->commands[reg->command_amount].name = name;
    reg->commands[reg->command_amount].description = description;
    reg->commands[reg->command_amount].func = func;
    reg->commands[reg->command_amount].args_amount = 0; // set if needed elsewhere

    reg->command_amount++;
}

Command* match_command(CommandRegistry* reg, const char* command_name)
{
    size_t i;
    
    if (!reg || !command_name) 
        return NULL;

    for (i = 0; i < reg->command_amount; ++i) 
    {
        if (strcmp(reg->commands[i].name, command_name) == 0)
            return &reg->commands[i];
    }
    return NULL;
}


//----------------------------------------
//  Main Loop
//----------------------------------------
void command_prompt_run(CommandRegistry* reg)
{
    char* input = NULL;
    char* command_name = NULL;
    char** tokens = NULL;
    char** args = NULL;
    size_t token_amount;
    Command* command = NULL;

    size_t args_count;

    // The context persists while the shell is running
    Context ctx;
    ctx.registry = reg;
    ctx.plugin_handles = NULL;
    ctx.plugin_count = 0;
    ctx.table = NULL;

    while (1)
    {
        get_input("> ", &input);

        if (input == NULL) {
            // EOF or read error: exit cleanly
            printf("\nEOF detected — exiting.\n");
            // free table and plugins if needed
            if (ctx.table) table_destroy(&ctx.table);
            for (size_t i = 0; i < ctx.plugin_count; ++i) {
                if (ctx.plugin_handles[i]) dlclose(ctx.plugin_handles[i]);
            }
            free(ctx.plugin_handles);
            return;
        }

        // skip empty lines
        if (input[0] == '\0') {
            free(input);
            continue;
        }

        tokens = split(input, ' ', &token_amount);
        free(input);
        input = NULL;

        if (token_amount == 0) {
            free(tokens);
            continue;
        }

        command_name = tokens[0];

        command = match_command(ctx.registry, command_name);

        if (command == NULL)
        {
            printf("Command '%s' not found\n", command_name);
            free(tokens);
            continue;
        }

        // Validate argument count
        args_count = (unsigned short)(token_amount - 1);
        if (args_count != (unsigned short)command->args_amount)
        {
            printf("Wrong argument amount for command '%s', expected: %zu\n",
                   command->name, command->args_amount);
            free(tokens);
            continue;
        }

        // Build args array pointer (const char** expected by CommandFunc)
        args = NULL;
        if (args_count > 0) {
            // tokens[1] .. tokens[token_amount-1]
            args = &tokens[1];
        }

        // Execute command
        command->func(&ctx, (const char**)args);

        // free token strings & array
        free(tokens);
    }
}


//----------------------------------------
//  Built-in Commands
//----------------------------------------

static void cmd_help(Context* context, const char** args)
{
    CommandRegistry* reg;

    size_t i;

    reg = context->registry;

    for(i = 0; i < reg->command_amount; i++)
        printf("%s - %s\n", reg->commands[i].name, reg->commands[i].description);
}

static void cmd_exit(Context* context, const char** args)
{
    table** t_ref = NULL;
    CommandRegistry* reg = NULL;

    t_ref = &(context->table);

    if (*t_ref != NULL)
        table_destroy(t_ref);

    reg = context->registry;

    free(reg->commands);
    free(reg);

    printf("Closing the program...\n");
    exit(OK.code);
}

static void cmd_load(Context* context, const char** args)
{
    table** t_ref = NULL;

    const char* filename;

    filename = args[0];

    t_ref = &(context->table);

    if (*t_ref != NULL)
        table_destroy(t_ref);

    *t_ref = table_load_csv(filename);

    if (*t_ref == NULL)
    {
        printf("Load failed.\n");
        return;
    }

    printf("Loaded table with %zu rows.\n", (*t_ref)->row_num);
}

static void cmd_save(Context* context, const char** args)
{
    table* t = NULL;

    const char* filename;

    filename = args[0];

    t = context->table;

    if (t == NULL)
    {
        printf("No table loaded.\n");
        return;
    }

    table_save_csv(t, filename);
    printf("Saved.\n");
}

Status process_coordinate(const char* coordinate, size_t* row, unsigned short* col)
{
    if (!isalpha(*coordinate))
        return ERROR;

    *col = col_letter_to_index(*coordinate);

    return processNaturalNum((coordinate) + 1, row);
}

Status get_subtable_coordinates(const char* coordinates, unsigned short* col1, size_t* row1, unsigned short* col2, size_t* row2)
{
    size_t count;
    char** split_coordinates;

    char* coordinate1 = NULL;
    char* coordinate2 = NULL;

    Status status;

    split_coordinates = split(coordinates, ':', &count);

    if (count != 2)
    {
        free(split_coordinates);
        return ERROR;
    }

    coordinate1 = split_coordinates[0];

    status = process_coordinate(coordinate1, row1, col1);
    
    if (status.code != OK.code)
    {
        free(split_coordinates);
        return ERROR;
    }

    coordinate2 = split_coordinates[1];

    free(split_coordinates);
    return process_coordinate(coordinate2, row2, col2);
}

static void cmd_show(Context* context, const char** args)
{
    table* t = NULL;

    char* value = NULL;
    const char* coordinates = NULL;

    unsigned short col1, col2;
    size_t row1, row2;
    size_t r, c;

    Status status;

    t = context->table;

    coordinates = args[0];

    if (t == NULL)
    {
        printf("No table loaded.\n");
        return;
    }

    status = get_subtable_coordinates(coordinates, &col1, &row1, &col2, &row2);
    if (status.code != OK.code)
    {
        printf("Invalid coordinates.\n");
        return;
    }

    if (col1 > t->col_num || row1 > t->row_num) 
    {
        printf("Coordinates out of table bounds.\n");
        return;
    }

    if (row1 > row2 || col1 > col2) 
    {
        printf("Invalid range: start must be <= end.\n");
        return;
    }

    // iterate rows and columns 
    for (r = row1; r <= row2; r++)
    {
        for (c = col1; c <= col2; c++)
        {
            value = table_get_cell_contents(t, r, (char)('A' + c), &status);
            if (value)
                printf("%s", value);
            // else print empty
            if (c != col2) printf(" | ");
        }
        printf("\n");
    }
}

typedef struct
{
    unsigned short col_index;
    const char* expected_value;
} filter_context;

static bool filter_predicate(const void* row_ptr, const void* ctx_ptr)
{
    const row* r = (const row*)row_ptr;
    const filter_context* ctx = (const filter_context*)ctx_ptr;

    const char* cell_value = r->cells[ctx->col_index];

    if (cell_value == NULL)
        return false;

    return strcmp(cell_value, ctx->expected_value) == 0;
}

bool is_char(const char* str)
{
    if (strlen(str) != 1)
        return false;
    
    if (!isalpha(str[0]))
        return false;

    return true;
}

static void cmd_filter(Context* context, const char** args)
{
    table* t = NULL;
    table* new_table = NULL;

    filter_context fctx;

    const char* column_arg;
    char col;

    t = context->table;

    if (t == NULL)
    {
        printf("No table loaded.\n");
        return;
    }

    column_arg = args[0];

    if (!is_char(column_arg))
    {
        printf("Invalid column argument: %s.\n", column_arg);
        return;
    }

    col = toupper(column_arg[0]);

    if (col < 'A' || col >= 'A' + t->col_num)
    {
        printf("Column %c is outside table range A-%c", col, (char) 'A' + t->col_num);
        return;
    }

    /* prepare predicate context */
    fctx.col_index = (unsigned short)(col - 'A');
    fctx.expected_value = args[1];

    /* call filter with efficient context */
    new_table = table_filter(t, filter_predicate, (void*) &fctx);

    /* Swap tables */
    table_destroy(&t);
    t = new_table;
}

int main() 
{
    CommandRegistry* reg = NULL;

    reg = create_command_registry();

    command_prompt_run(reg);
}