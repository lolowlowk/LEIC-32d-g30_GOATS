#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../table/table.h"
#include "../../table/table_io.h"
#include "../../table/table_filter.h"

// Simple test function
static bool is_cheaper_than_1(const void *rowPtr, const void *ctx)
{
    const row *r = rowPtr;
    int price_col = *(const int *)ctx;

    if (!r || !r->cells)
        return false;

    if (price_col < 0)
        return false;

    if (r->cells[price_col] == NULL)
        return false;

    double price = atof(r->cells[price_col]);
    return price < 1.0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <output.csv> <input.csv>\n", argv[0]);
        return 1;
    }

    table *t = table_load_csv(argv[2]);
    if (!t)
    {
        fprintf(stderr, "Error loading CSV\n");
        return 1;
    }

    int price_col = col_letter_to_index('E');
    if (price_col < 0 || price_col >= t->col_num)
    {
        fprintf(stderr, "Invalid price column\n");
        table_destroy(&t);
        return 1;
    }

    table *filtered = table_filter(t, is_cheaper_than_1, &price_col);
    if (!filtered)
    {
        fprintf(stderr, "Error filtering table\n");
        table_destroy(&t);
        return 1;
    }

    table_save_csv(filtered, argv[1]);

    table_destroy(&t);
    table_destroy(&filtered);

    return 0;
}
