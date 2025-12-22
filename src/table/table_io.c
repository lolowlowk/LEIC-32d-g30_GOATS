#include "table.h"
#include "string.h"
#include "stdlib.h"

#include "../file_utils/file_utils.h"
#include "../util/string_util.h"
#include "../util/memory.h"

#define INITIAL_TOKENS_CAP 4

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

char** parse_csv_line(const char* line, size_t* count)
{
    size_t num_tokens, tokens_cap, len;
    char** tokens = NULL;
    const char* start = NULL;
    const char* ptr = NULL;

    num_tokens = 0;
    tokens_cap = INITIAL_TOKENS_CAP;

    safeMalloc((void**)&tokens, tokens_cap * sizeof(char*));

    start = line;
    ptr = line;

    while (*ptr != '\0') 
    {
        if (*ptr == '"') 
        {
            ptr++;  // skip opening quote
            start = ptr;

            while (*ptr != '"')
                ptr++; 

            len = ptr - start;
            add_token(&tokens, &num_tokens, &tokens_cap, start, len);

            ptr++;  // skip closing quote
            if (*ptr == ',')
                ptr++;  // skip separator

            start = ptr;
            continue;
        }

        if (*ptr == ',') 
        {
            len = ptr - start;
            add_token(&tokens, &num_tokens, &tokens_cap, start, len);

            ptr++;      // skip separator
            start = ptr;
            continue;
        }

        ptr++;
    }

    // Handle last token if line doesn't end with separator
    if (ptr != start) 
    {
        len = ptr - start;
        add_token(&tokens, &num_tokens, &tokens_cap, start, len);
    }

    *count = num_tokens;
    return tokens;
}

// Load CSV into table
table* table_load_csv(const char* filename)
{
    FILE* file = NULL;
    table* t = NULL;

    unsigned short col_count;
    unsigned short col;

    char** tokens = NULL;
    char* line = NULL;
    bool isOver = false;

    file = open_file(filename, "r");
    if (file == NULL)
        return NULL;

    // --- Read first line to determine column count ---
    readLine(file, &line, &isOver);

	if (line == NULL || line[0] == '\0') {
		fclose(file);
		free(line);
		announceStatus(INVALID_TABLE);   // ou o erro que usares
		return NULL;
	}

	tokens = parse_csv_line(line, (size_t*) &col_count);

	if (col_count == 0) {
		fclose(file);
		free(line);
		free(tokens);
		announceStatus(INVALID_TABLE);
		return NULL;
	}

    // Create table
    t = table_create(col_count);

    // Add first row (header or data, depending on your design)
    table_add_row(t);
    for (col = 0; col < col_count; col++)
    {
		char *clean = trim(tokens[col]);
        table_set_cell(t, 0, 'A' + col, clean);
    }

    // Free first line tokens
    for (col = 0; col < col_count; col++)
        free(tokens[col]);
    free(tokens);
    free(line);

    // --- Process remaining lines ---
    while (!isOver)
    {
        readLine(file, &line, &isOver);
        if (isOver) { free(line); break; }

        tokens = parse_csv_line(line, (size_t*) &col_count);

        table_add_row(t);
        for (col = 0; col < col_count; col++)
        {
			char *clean = trim(tokens[col]);
            table_set_cell(t, t->row_num - 1, 'A' + col, clean);
        }

        for (col = 0; col < col_count; col++)
            free(tokens[col]);
        free(tokens);
        free(line);
    }

    fclose(file);
    return t;
}

char quote_check(char* cell)
{
    char* p = NULL;

    bool has_separator = true;

    p = cell;
    while (*p != '"')
    {
        if (*p == '\0')
        {
            if (has_separator)
                return ',';

            return '\0';
        }

        if (*p == ',') 
            has_separator = false;

        p++;
    }

    return '"';
}

void write_with_escaped_quotes(FILE* f, char* cell)
{
    char* p = NULL;

    fprintf(f, "%c", '"');
    p = cell;
    while (*p != '\0')
    {
        fprintf(f, "%c", *p);
        
        if (*p == '"')
            fprintf(f, "%c", '"');

        p++;
    }
    fprintf(f, "%c", '"');
}

// Save the table to a CSV file
void table_save_csv(const table* t, const char* filename)
{
    FILE* file = NULL;

    size_t r;
    unsigned short col;

    char* cell = NULL;
    char quote_action;

    file = open_file(filename, "w");
    if (file == NULL) 
        return;

    if (t == NULL) 
    {
        announceStatus(INVALID_TABLE);
        return;
    }

    // Write each row
    for (r = 0; r < t->row_num; r++) 
    {
        for (col = 0; col < t->col_num; col++) 
        {
            cell = t->rows[r].cells[col];
            if (cell != NULL) 
            {   
                quote_action = quote_check(cell);

                if (quote_action == ',') 
                    fprintf(file, "\"%s\"", cell);

                else if (quote_action == '"')
                    fprintf(file, "%s", cell);

                else
                    write_with_escaped_quotes(file, cell);

            }

            if (col < t->col_num - 1) 
                fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
