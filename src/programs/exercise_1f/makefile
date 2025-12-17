SRC = filter_program.c \
      ../../table/table.c \
      ../../table/table_io.c \
      ../../table/table_filter.c \
      ../../util/memory.c \
      ../../util/string_util.c \
      ../../util/user_input.c \
      ../../status/status.c \
      ../../file_utils/file_utils.c

all: filter_program

filter_program: $(SRC)
	gcc -Wall -Wextra -O2 -I../../table -I../../util -o filter_program $(SRC)

clean:
	rm -f filter_program
