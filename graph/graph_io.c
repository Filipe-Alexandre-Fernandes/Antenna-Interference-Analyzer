/** @file graph_io.c
 * @brief Antenna graph project, adapted from Filipe Fernandes' May 2025 submission.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "graph_io.h"

Vertex* load_matrix(const char* filename, int* rows, int* columns) {
    if (!filename || !rows || !columns) return NULL;
    *rows = *columns = 0;
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Could not open file.\n");
        return NULL;
    }

    Vertex* list = NULL;
    char row[256];
    int l = 0;
    int max_columns = 0;

    while (fgets(row, sizeof(row), f)) {
        size_t len = strcspn(row, "\r\n");
        /* Reject oversized/blank rows instead of silently splitting the map. */
        if (len == 0 || len > 253) {
            fprintf(stderr, "Map rows must contain 1 to 253 cells.\n");
            free_graph(list);
            fclose(f);
            return NULL;
        }
        row[len] = '\0';
        if ((int)len > max_columns) max_columns = (int)len;

        for (int c = 0; row[c] != '\0'; c++) {
            if (row[c] != '.') {
                Vertex* v = create_vertex(l, c, row[c]);
                if (!v) {
                    fclose(f);
                    free_graph(list);  
                    return NULL;
                }
                list = insert_vertex(list, v);
            }
        }
        l++;
    }

    if (ferror(f)) {
        free_graph(list);
        fclose(f);
        return NULL;
    }
    fclose(f);
    *rows = l;
    *columns = max_columns;
    return list;
}

bool save_graph_binary(const char* filename, Vertex* list, Interference* interference) {
    if (!filename || !list) return false;

    FILE* f = fopen(filename, "wb");
    if (!f) return false;

    int vertex_count = 0;
    for (Vertex* v = list; v; v = v->next) vertex_count++;
    if (fwrite(&vertex_count, sizeof(int), 1, f) != 1) goto write_error;

    for (Vertex* v = list; v; v = v->next) {
        if (fwrite(&v->row, sizeof(int), 1, f) != 1) goto write_error;
        if (fwrite(&v->column, sizeof(int), 1, f) != 1) goto write_error;
        if (fwrite(&v->frequency, sizeof(char), 1, f) != 1) goto write_error;

        int adjacency_count = 0;
        for (Adjacency* a = v->adjacent; a; a = a->next) adjacency_count++;
        if (fwrite(&adjacency_count, sizeof(int), 1, f) != 1) goto write_error;

        for (Adjacency* a = v->adjacent; a; a = a->next) {
            if (fwrite(&a->row, sizeof(int), 1, f) != 1) goto write_error;
            if (fwrite(&a->column, sizeof(int), 1, f) != 1) goto write_error;
            if (fwrite(&a->frequency, sizeof(char), 1, f) != 1) goto write_error;
        }
    }

    int interference_count = 0;
    for (Interference* i = interference; i; i = i->next) interference_count++;
    if (fwrite(&interference_count, sizeof(int), 1, f) != 1) goto write_error;

    for (Interference* i = interference; i; i = i->next) {
        if (fwrite(&i->row, sizeof(int), 1, f) != 1) goto write_error;
        if (fwrite(&i->column, sizeof(int), 1, f) != 1) goto write_error;
    }

    return fclose(f) == 0;

write_error:
    fclose(f);
    return false;
}

bool load_graph_binary(const char* filename, Vertex** list) {
    if (!filename || !list || *list) return false;

    FILE* f = fopen(filename, "rb");
    if (!f) return false;

    int total;
    if (fread(&total, sizeof(int), 1, f) != 1) goto read_error;

    if (total < 0) goto read_error;

    for (int i = 0; i < total; i++) {
        int row, column;
        char freq;
        if (fread(&row, sizeof(int), 1, f) != 1) goto read_error;
        if (fread(&column, sizeof(int), 1, f) != 1) goto read_error;
        if (fread(&freq, sizeof(char), 1, f) != 1) goto read_error;

        Vertex* v = create_vertex(row, column, freq);
        if (!v) goto read_error;

        *list = insert_vertex(*list, v);

        int adjacency_count;
        if (fread(&adjacency_count, sizeof(int), 1, f) != 1) goto read_error;
        if (adjacency_count < 0) goto read_error;
        for (int j = 0; j < adjacency_count; j++) {
            int adjacent_row, adjacent_column;
            char adjacent_frequency;
            if (fread(&adjacent_row, sizeof(int), 1, f) != 1) goto read_error;
            if (fread(&adjacent_column, sizeof(int), 1, f) != 1) goto read_error;
            if (fread(&adjacent_frequency, sizeof(char), 1, f) != 1) goto read_error;

            Adjacency* a = create_adjacency(adjacent_row, adjacent_column, adjacent_frequency);
            if (!a) goto read_error;
            v->adjacent = insert_adjacency(v->adjacent, a);
        }
    }

    fclose(f);
    return true;

read_error:
    fclose(f);
    free_graph(*list);
    *list = NULL;
    return false;
}
