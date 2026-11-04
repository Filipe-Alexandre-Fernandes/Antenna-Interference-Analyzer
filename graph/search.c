/** @file search.c
 * @brief Antenna graph project, adapted from Filipe Fernandes' May 2025 submission.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

bool dfs(Vertex* list, int row, int column, bool* visited, int total) {

    Vertex* origin = find_vertex(list, row, column);
    if (!origin || total <= 0 || total < count_vertices(list)) return false;

    int idx = 0;
    for (Vertex* v = list; v; v = v->next, idx++) {
        if (v == origin) break;
        
    }

    if (!visited || idx >= total || visited[idx]) return false;

    visited[idx] = true;
    printf("DFS visit: (%d,%d) %c\n", origin->row, origin->column, origin->frequency);

    for (Adjacency* a = origin->adjacent; a; a = a->next) {
        dfs(list, a->row, a->column, visited, total);
    }
    return true;
}

bool bfs(Vertex* list, int row, int column, int total) {
    Vertex* origin = find_vertex(list, row, column);
    if (!origin || total <= 0 || total < count_vertices(list)) return false;

    bool* visited = calloc(total, sizeof(bool));
    Vertex** queue = malloc(total * sizeof(Vertex*));
    if (!visited || !queue) {
        free(visited);
        free(queue);
        return false;
    }
    int front = 0, back = 0;

    int origin_index = 0;
    for (Vertex* v = list; v != origin; v = v->next) origin_index++;
    visited[origin_index] = true;
    queue[back++] = origin;

    while (front < back) {
        Vertex* current = queue[front++];

        printf("BFS visit: (%d,%d) %c\n", current->row, current->column, current->frequency);

        for (Adjacency* a = current->adjacent; a; a = a->next) {
            Vertex* neighbor = find_vertex(list, a->row, a->column);
            if (!neighbor) continue;

            int i = 0;
            for (Vertex* v = list; v; v = v->next, i++) {
                if (v == neighbor && !visited[i]) {
                    /* Mark on enqueue: each vertex occupies at most one queue slot. */
                    visited[i] = true;
                    queue[back++] = neighbor;
                    break;
                }
            }
        }
    }

    free(queue);
    free(visited);
    return true;
}
