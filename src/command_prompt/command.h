#ifndef COMMAND_H
#define COMMAND_H

#include <stddef.h>
#include "../table/table.h"


// ---------------------------------------------------------
// Forward declarations (required for type references)
// ---------------------------------------------------------

struct CommandRegistry;      // Forward declare for Context
struct Command;              // Sometimes useful as well

typedef struct CommandRegistry CommandRegistry;
typedef struct Command Command;


// ---------------------------------------------------------
// Context structure
// ---------------------------------------------------------

typedef struct {
    table *table;
    CommandRegistry *registry;
    void **plugin_handles;
    size_t plugin_count;
} Context;


// ---------------------------------------------------------
// Command function signature
// ---------------------------------------------------------

typedef void (*CommandFunc)(Context* ctx, const char** args);


// ---------------------------------------------------------
// Command structure
// ---------------------------------------------------------

struct Command {
    const char* name;
    const char* description;
    CommandFunc func;
    size_t args_amount;
};


// ---------------------------------------------------------
// Command registry structure
// ---------------------------------------------------------

struct CommandRegistry {
    Command* commands;
    size_t command_amount;
};


// ---------------------------------------------------------
// Public API
// ---------------------------------------------------------

CommandRegistry* create_command_registry();
void register_command(CommandRegistry* registry,
                      const char* name,
                      const char* description,
                      CommandFunc func,
                      size_t args_amount);

void command_prompt_run(CommandRegistry* registry);


// ---------------------------------------------------------
// Plug-in structure
// ---------------------------------------------------------

typedef void (*plugin_init_func)(CommandRegistry* registry);

#endif // COMMAND_H

