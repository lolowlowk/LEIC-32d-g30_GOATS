#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../table/table.h"
#include "../../table/table_io.h"
#include "../../util/memory.h"
#include "../../status/status.h"
#include "../../file_utils/file_utils.h"
#include "../../util/string_util.h"
#include "../../util/memory.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <output.csv> <input.csv>\n", argv[0]);
        return 1;
    }

    struct table *t = table_load_csv(argv[2]);
    if (!t) {
        fprintf(stderr, "Error loading input CSV.\n");
        return 1;
    }

    table_save_csv(t, argv[1]);

    table_destroy(&t);
    return 0;
}
