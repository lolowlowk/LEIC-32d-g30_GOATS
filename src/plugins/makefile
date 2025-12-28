# --------------------------------------------------
# Compiler and flags
# --------------------------------------------------
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -fPIC -I../table -I../util -I../status -I../command_prompt
LDFLAGS = -shared

# --------------------------------------------------
# Directories
# --------------------------------------------------
PLUGIN_SRC_DIR = plugins
PLUGIN_LIB_DIR = plugins

# --------------------------------------------------
# Sources, objects, shared libraries
# --------------------------------------------------
PLUGIN_SRCS := $(wildcard $(PLUGIN_SRC_DIR)/*.c)
PLUGIN_OBJS := $(patsubst $(PLUGIN_SRC_DIR)/%.c,$(PLUGIN_SRC_DIR)/%.o,$(PLUGIN_SRCS))
PLUGIN_SO   := $(patsubst $(PLUGIN_SRC_DIR)/%.c,$(PLUGIN_LIB_DIR)/%.so,$(PLUGIN_SRCS))

# --------------------------------------------------
# Default target: build all plugin .so files
# --------------------------------------------------
all: $(PLUGIN_SO)

# --------------------------------------------------
# Build shared objects from object files
# --------------------------------------------------
$(PLUGIN_LIB_DIR)/%.so: $(PLUGIN_SRC_DIR)/%.o
	$(CC) $(LDFLAGS) -o $@ $<

# --------------------------------------------------
# Compile plugin object files
# --------------------------------------------------
$(PLUGIN_SRC_DIR)/%.o: $(PLUGIN_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------
# Clean
# --------------------------------------------------
clean:
	rm -f $(PLUGIN_OBJS) $(PLUGIN_SO)

# --------------------------------------------------
# Phony targets
# --------------------------------------------------
.PHONY: all clean
