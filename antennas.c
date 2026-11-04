/** @file antennas.c
 * @brief Antenna list operations and interference analysis.
 * @author Filipe Fernandes
 */
#include "antennas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "antennas.h"
/** @brief Allocate an antenna at one-based column x and row y. */

Antenna* create_antenna(char freq, int x, int y) {
    Antenna* new_antenna = (Antenna*)malloc(sizeof(Antenna));
    if (new_antenna != NULL) {
        new_antenna->frequency = freq;
        new_antenna->x = x;
        new_antenna->y = y;
        new_antenna->next = NULL;
    }
    return new_antenna;
}
/** @brief Insert an antenna sorted by row, then column. */

Antenna* insert_antenna_sorted(Antenna* h, Antenna* new_antenna) {
    if (!new_antenna || find_antenna(h, new_antenna->frequency, new_antenna->x, new_antenna->y))
        return h;

    if (!h || (new_antenna->y < h->y || (new_antenna->y == h->y && new_antenna->x < h->x))) {
        new_antenna->next = h;
        return new_antenna;
    }

    Antenna *current = h, *previous = NULL;
    while (current && (current->y < new_antenna->y || (current->y == new_antenna->y && current->x < new_antenna->x))) {
        previous = current;
        current = current->next;
    }
    previous->next = new_antenna;
    new_antenna->next = current;
    return h;
}
/** @brief Find an antenna matching frequency and coordinates. */

Antenna* find_antenna(Antenna* h, char freq, int x, int y) {
    while (h) {
        if (h->frequency == freq && h->x == x && h->y == y)
            return h;
        h = h->next;
    }
    return NULL;
}
/** @brief Remove and free a matching antenna. */

Antenna* remove_antenna(Antenna* h, char freq, int x, int y) {
    Antenna *aux = h, *previous = NULL;
    while (aux && (aux->frequency != freq || aux->x != x || aux->y != y)) {
        previous = aux;
        aux = aux->next;
    }
    if (!aux) return h;

    if (!previous)
        h = aux->next;
    else
        previous->next = aux->next;

    free(aux);
    return h;
}
/** @brief Print frequencies and one-based (row, column) coordinates. */

void list_antennas(Antenna* h) {
    while (h) {
        printf("Frequency: %c | Coordinates (row, column): (%d, %d)\n", h->frequency, h->y, h->x);
        h = h->next;
    }
}
/** @brief Print same-frequency antinodes at A - (B-A) and B + (B-A). */

void print_interference_grid(Antenna* list, int rows, int columns) {
    char grid[rows + 1][columns + 1];
    
    for (int y = 1; y <= rows; y++) {
        for (int x = 1; x <= columns; x++) {
            grid[y][x] = '.';
        }
    }
    
    for (Antenna* a = list; a != NULL; a = a->next) {
        if (a->x >= 1 && a->x <= columns && a->y >= 1 && a->y <= rows)
            grid[a->y][a->x] = a->frequency;
    }
    
    for (Antenna* a1 = list; a1 != NULL; a1 = a1->next) {
        for (Antenna* a2 = a1->next; a2 != NULL; a2 = a2->next) {
            if (a1->frequency == a2->frequency) {
                int dx = a2->x - a1->x;
                int dy = a2->y - a1->y;
                
                int x1 = a1->x - dx;
                int y1 = a1->y - dy;
                if (x1 >= 1 && x1 <= columns && y1 >= 1 && y1 <= rows && grid[y1][x1] == '.')
                    grid[y1][x1] = '#';
                
                int x2 = a2->x + dx;
                int y2 = a2->y + dy;
                if (x2 >= 1 && x2 <= columns && y2 >= 1 && y2 <= rows && grid[y2][x2] == '.')
                    grid[y2][x2] = '#';
            }
        }
    }
    
printf("\nInterference grid (original input layout):\n");
for (int y = 1; y <= rows; y++) {
    for (int x = 1; x <= columns; x++) {
        printf("%c", grid[y][x]);
    }
    printf("\n");
}
}
/** @brief Load a text grid; return its antenna list and dimensions. */

Antenna* load_antennas_from_file(const char* filename, int* rows, int* columns) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Could not open input file");
        return NULL;
    }
    printf("Input file opened. Reading rows...\n");
    char line[128];
    int row_count = 0;
    int max_columns = 0;
    Antenna* list = NULL;
    while (fgets(line, sizeof(line), f)) {
        int length = 0;
        for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
            char ch = line[i];
            if (ch != '.') {
                printf("Antenna found: %c at (%d, %d)\n", ch, row_count + 1, i + 1);
                Antenna* new_antenna = create_antenna(ch, i + 1, row_count + 1);
                list = insert_antenna_sorted(list, new_antenna);
            }          
            length++;
        }
        if (length > max_columns) max_columns = length;
        row_count++;
    }
    fclose(f);
    *rows = row_count;
    *columns = max_columns;
    if (!list) {
        printf("No antennas were loaded from the input file!\n");
    }
    return list;
}
/** @brief Write nonempty cells as native BinaryRecord structures. */

 bool save_antennas_binary(const char* filename, Antenna* list) {
    if (!list) return false;
    FILE* fp = fopen(filename, "wb");
    if (!fp) return false;
    
    int maxX = 0, maxY = 0;
    for (Antenna* a = list; a != NULL; a = a->next) {
        if (a->x > maxX) maxX = a->x;
        if (a->y > maxY) maxY = a->y;
    }
    
    char grid[maxY + 1][maxX + 1];
    for (int y = 1; y <= maxY; y++)
        for (int x = 1; x <= maxX; x++)
            grid[y][x] = '.';
    
    for (Antenna* a = list; a != NULL; a = a->next) {
        grid[a->y][a->x] = a->frequency;
    }
    
    for (Antenna* a1 = list; a1 != NULL; a1 = a1->next) {
        for (Antenna* a2 = a1->next; a2 != NULL; a2 = a2->next) {
            if (a1->frequency == a2->frequency) {
                int dx = a2->x - a1->x;
                int dy = a2->y - a1->y;
                int x1 = a1->x - dx;
                int y1 = a1->y - dy;
                int x2 = a2->x + dx;
                int y2 = a2->y + dy;
                if (x1 >= 1 && x1 <= maxX && y1 >= 1 && y1 <= maxY && grid[y1][x1] == '.')
                    grid[y1][x1] = '#';

                if (x2 >= 1 && x2 <= maxX && y2 >= 1 && y2 <= maxY && grid[y2][x2] == '.')
                    grid[y2][x2] = '#';
            }
        }
    }
    
    BinaryRecord record;
    for (int y = 1; y <= maxY; y++) {
        for (int x = 1; x <= maxX; x++) {
            if (grid[y][x] != '.') {
                record.type = grid[y][x];
                record.x = x;
                record.y = y;
                fwrite(&record, sizeof(BinaryRecord), 1, fp);
            }
        }
    }
    fclose(fp);
    return true;
}

