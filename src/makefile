# Library object files
LIB_OBJS = \
	table/table.o \
	table/table_io.o \
	table/table_filter.o \
	util/memory.o \
	util/string_util.o \
	util/file_utils.o \
	status/status.o

# Programs
FILTER_PROGRAM = programs/exercise_1f/filter_program
REPLICA_PROGRAM = programs/exercise_1d/replica_program

# Default target
all: libtable.so $(FILTER_PROGRAM) $(REPLICA_PROGRAM)

# ---------- Shared library ----------
libtable.so: $(LIB_OBJS)
	gcc -shared -o $@ $^

# ---------- Object files ----------
%.o: %.c
	gcc -Wall -Wextra -O2 -fPIC -I./table -I./util -I./status -c $< -o $@

# ---------- Programs ----------
$(FILTER_PROGRAM): programs/exercise_1f/filter_program.c libtable.so
	gcc -Wall -Wextra -O2 -I./table -I./util -I./status $< -L. -ltable -o $@

$(REPLICA_PROGRAM): programs/exercise_1d/replica_program.c libtable.so
	gcc -Wall -Wextra -O2 -I./table -I./util -I./status $< -L. -ltable -o $@

# ---------- Run helpers ----------
run-filter:
	LD_LIBRARY_PATH=. ./$(FILTER_PROGRAM) programs/exercise_1f/filtered.csv programs/exercise_1f/frutas.csv

run-replica:
	LD_LIBRARY_PATH=. ./$(REPLICA_PROGRAM) programs/exercise_1d/created.csv programs/exercise_1d/frutas.csv

# ---------- Cleanup ----------
clean:
	rm -f $(LIB_OBJS) $(FILTER_PROGRAM) $(REPLICA_PROGRAM) libtable.so
