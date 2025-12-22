# --------------------------------------------------
# Compiler and flags
# --------------------------------------------------
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g \
         -I./table -I./util -I./status -I./file_utils -I./command_prompt
LDFLAGS = -L. -ltable -ldl

# --------------------------------------------------
# Directories
# --------------------------------------------------
TARGET_DIR = command_prompt
TARGET = $(TARGET_DIR)/table_shell

# --------------------------------------------------
# Sources
# --------------------------------------------------
SRC = \
	command_prompt/command_prompt.c \
	util/memory.c \
	util/string_util.c \
	util/user_input.c

OBJ = $(SRC:.c=.o)

# --------------------------------------------------
# Rules
# --------------------------------------------------

all: $(TARGET)

# Link executable
$(TARGET): $(OBJ) libtable.so
	$(CC) $(OBJ) -o $@ $(LDFLAGS) -rdynamic

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)

# Run (LD_LIBRARY_PATH aponta para a pasta onde está libtable.so)
run: all
	LD_LIBRARY_PATH=$(shell pwd) ./$(TARGET)

.PHONY: all clean run
