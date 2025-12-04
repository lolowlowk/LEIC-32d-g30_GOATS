/*
    1.
    a) 
    Defina a struct table para suporte em memória dos dados de uma tabela. A tabela tem duas dimensões,
    as colunas são identificadas por letras e as linhas são identificadas por números. O número de colunas é
    fixado na criação da tabela, num máximo de 26 colunas. O número de linhas é dinâmico, pode aumentar ou
    diminuir ao longo do processamento. Na concretização da struct table deve-se minimizar a memória
    alocada e não utilizada.
    struct table { …campos a definir… };
*/

#include <stdio.h>
#include <stdlib.h>


#define MAX_COLS 26

#define MALLOC_FAIL_ERROR_CODE -1
#define REALLOC_FAIL_ERROR_CODE -2
#define INVALID_COLUMN_ERROR_CODE -3
#define INVALID_TABLE_ERROR_CODE -4
#define INVALID_ROW_ERROR_CODE -5


typedef struct {
    char*** data;           // [row][col] = string
    size_t row_num;         // dynamic
    unsigned short col_num; // <= 26, fixed
} table;

/*
    [
        [[], [], [], []],
        [[], [], [], []],
        [[], [], [], []]
    ]
*/

table* table_create(unsigned const short cols) 
{
    table* pNew_table = NULL;

    if (cols <= 0 || cols > 26) 
    {
        fprintf(stderr, "\nInvalid number of columns: %hu (allowed: 1-%d)\n", cols, MAX_COLS);
        return NULL;
    }

    safeMalloc((void**) &pNew_table, sizeof(table));

    pNew_table->data = NULL;
    pNew_table->row_num = 0;
    pNew_table->col_num = cols;

    return pNew_table;
}

void table_add_row(table* t)
{
    int col;

    if (t == NULL)
    {
        fprintf(stderr, "\nInvalid table\n");
        return;
    }

    safeRealloc((void**) &(t->data), (t->row_num + 1) * sizeof(char**));

    safeMalloc((void**) &(t->data[t->row_num]), t->col_num * sizeof(char*));

    for (int col = 0; col < t->col_num; col++)
        t->data[t->row_num][col] = NULL;  

    t->row_num++;
}

int col_letter_to_index(const char col) 
{
    if (col >= 'A' && col <= 'Z') return col - 'A';
    if (col >= 'a' && col <= 'z') return col - 'a';
    return INVALID_COLUMN_ERROR_CODE;
}

int table_set_cell(table* t, const size_t row, const char col_letter, const char* value)
{   
    int error_code;
    
    error_code = 0;
    char** cell = table_get_cell_ref(t, row, col_letter, &error_code);

    if (cell == NULL) 
        return error_code;

    free(*cell);

    if (value == NULL) 
    {
        *cell = NULL;
        return 0;
    }

    safeMalloc((void**) cell, strlen(value) + 1);
    strcpy(*cell, value);

    return 0;
}

char* table_get_cell(const table* t, const size_t row_num, const char col_letter, int* error_code)
{
    char** cell = table_get_cell_ref(t, row_num, col_letter, error_code);

    if (cell == NULL)
        return NULL;

    return *cell; // may be NULL
}


char** table_get_cell_ref(const table* t, const size_t row, const char col_letter, int* error_code) 
{
    if (t == NULL) 
    {   
        *error_code = INVALID_TABLE_ERROR_CODE;
        return NULL;
    }

    if (row >= t->row_num) 
    {
        *error_code = INVALID_ROW_ERROR_CODE;
        return NULL;
    }

    int col = col_letter_to_index(col_letter);
    if (col < 0) 
    {
        *error_code = INVALID_COLUMN_ERROR_CODE;
        return NULL;
    }

    return &(t->data[row][col]);
}

int table_remove_row(table* t, size_t index);

void table_destroy(table* t) 
{

}


//////////////////////////////////////////////////////////////////////

void handleAllocError(const int code)
{
    fprintf(stderr, "\nMemory allocation error. Exiting...\n");
    exit(code);
}

void safeMalloc(void** ptr, const size_t cap)
{
    void* tmp = malloc(cap);
    if (tmp == NULL)
        handleAllocError(MALLOC_FAIL_ERROR_CODE);
    *ptr = tmp;
}

void safeRealloc(void** ptr, const size_t cap)
{
    void *tmp = realloc(*ptr, cap);
    if (tmp == NULL)
        handleAllocError(REALLOC_FAIL_ERROR_CODE);
    *ptr = tmp;
}