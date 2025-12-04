#include "table.h"
#include "../io/file_utils.h"

/*
    Programe a função table_load_csv que cria uma tabela (uma instância de struct table) preenchida com
    os dados lidos de um ficheiro em formato CSV. Considere que o ficheiro está sempre bem formado.

    FICHEIRO.CSV
    fornecedor,fruta,quantidade,calibre,preco
    joaquim,maca,20,50,1.2
    manuel,melancia,100,200,0.64
    humberto, amora, 10, 12, 1.1
    ismael, figo, 30, 25, 5.0
    francisco, uva, 1000, 10, 0.5
    faustino, marmelo, 70, 50, 0.99
*/

// Split a line into tokens based on separator
// Returns a dynamically allocated array of strings and sets *count to the number of tokens
char** split_line(const char* line, const char separator, size_t* count)
{
    size_t capacity = 4;
    size_t num_tokens = 0;
    char** tokens = NULL;

    safeMalloc((void**)&tokens, capacity * sizeof(char*));

    const char* start = line;
    const char* ptr = line;

    while (*ptr) 
    {
        if (*ptr == separator || *ptr == '\n' || *ptr == '\0') 
        {
            size_t len = ptr - start;
            char* token = NULL;
            safeMalloc((void**)&token, len + 1);
            strncpy(token, start, len);
            token[len] = '\0';

            if (num_tokens >= capacity) 
            {
                capacity *= 2;
                safeRealloc((void**)&tokens, capacity * sizeof(char*));
            }

            tokens[num_tokens++] = token;

            start = ptr + 1;
        }
        ptr++;
    }

    *count = num_tokens;
    return tokens;
}

// Load CSV into table
table* table_load_csv(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) 
        return NULL;

    char* line = NULL;
    bool isOver = false;

    // Read first line to get number of columns
    readLine(file, &line, &isOver);
    if (!line) 
    {
        fclose(file);
        return NULL;
    }

    size_t col_count;
    char** headers = split_line(line, ',', &col_count);
    free(line);

    table* t = table_create((unsigned short)col_count);
    if (!t) 
    {
        for (size_t i = 0; i < col_count; i++) free(headers[i]);
        free(headers);
        fclose(file);
        return NULL;
    }

    for (size_t i = 0; i < col_count; i++) free(headers[i]);
    free(headers);

    // Read remaining lines
    while (!isOver) 
    {
        readLine(file, &line, &isOver);
        if (!line) break;

        table_add_row(t);

        size_t token_count;
        char** tokens = split_line(line, ',', &token_count);

        for (size_t c = 0; c < col_count && c < token_count; c++)
            table_set_cell(t, t->row_num - 1, 'A' + (char)c, tokens[c]); // table_set_cell makes a copy

        // Free tokens array
        for (size_t c = 0; c < token_count; c++)
            free(tokens[c]);
        free(tokens);
        free(line);
    }

    fclose(file);
    return t;
}


// Save the table to a CSV file
void table_save_csv(const table* t, const char* filename)
{
    if (!t || !filename) return;

    FILE* file = fopen(filename, "w");
    if (!file) return;

    // Write header row: A, B, C, ... (optional, or just leave empty)
    for (unsigned short c = 0; c < t->col_num; c++) 
    {
        fprintf(file, "%c", 'A' + c);
        if (c < t->col_num - 1) fprintf(file, ",");
    }
    fprintf(file, "\n");

    // Write each row
    for (size_t r = 0; r < t->row_num; r++) 
    {
        for (unsigned short c = 0; c < t->col_num; c++) 
        {
            char* cell = t->rows[r].cells[c];
            if (cell) 
            {
                // Escape double quotes
                for (char* p = cell; *p; p++) 
                    if (*p == '"') fprintf(file, "\"\"");
                
                // Wrap in quotes if needed
                bool needsQuotes = strchr(cell, ',') || strchr(cell, '"');
                if (needsQuotes) fprintf(file, "\"%s\"", cell);
                else fprintf(file, "%s", cell);
            }

            if (c < t->col_num - 1) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
