SRC = replica_program.c \
      ../../table/table.c \
      ../../table/table_io.c \
      ../../table/table_filter.c \
      ../../util/memory.c \
      ../../util/string_util.c \
      ../../util/user_input.c \
      ../../status/status.c \
      ../../file_utils/file_utils.c

all: replica_program

replica_program: $(SRC)
	gcc -Wall -Wextra -O2 -I../../table -I../../util -o replica_program $(SRC)

clean:
	rm -f replica_program
