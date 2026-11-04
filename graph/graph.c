/** @file graph.c
 * @brief Antenna graph project, adapted from Filipe Fernandes' May 2025 submission.
 */
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

Vertex* create_vertex(int row, int column, char frequency) {
    Vertex* v = malloc(sizeof(Vertex));
    if (!v) return NULL;
    v->row = row;
    v->column = column;
    v->frequency = frequency;
    v->adjacent = NULL;
    v->next = NULL;
    return v;
}

Vertex* insert_vertex(Vertex* list, Vertex* new_vertex) {
    if (!new_vertex) return list;
    new_vertex->next = list;
    return new_vertex;
}

Vertex* find_vertex(Vertex* list, int row, int column) {
    while (list) {
        if (list->row == row && list->column == column)
            return list;
        list = list->next;
    }
    return NULL;
}

int count_vertices(Vertex* list) {
    int total = 0;
    for (Vertex* v = list; v != NULL; v = v->next) {
        total++;
    }
    return total;
}

Adjacency* create_adjacency(int row, int column, char frequency) {
    Adjacency* a = malloc(sizeof(Adjacency));
    if (!a) return NULL;
    a->row = row;
    a->column = column;
    a->frequency = frequency;
    a->next = NULL;
    return a;
}

Adjacency* insert_adjacency(Adjacency* adjacency_list, Adjacency* new_item) {
    if (!new_item) return adjacency_list;
    new_item->next = adjacency_list;
    return new_item;
}

bool list_antennas(Vertex* list) {
    if (!list) {
        printf("No antennas loaded.\n");
        return false;
    }
    int i = 0;
    while (list) {
        printf("Antenna %d -> Coordinates: (%d, %d), Frequency: %c\n", i++, list->row, list->column, list->frequency);
        list = list->next;
    }
    return true;
}

Interference* calculate_interference(Vertex* list, int rows, int columns) {
    Interference* head = NULL;
    for (Vertex* a1 = list; a1; a1 = a1->next) {
        for (Vertex* a2 = a1->next; a2; a2 = a2->next) {
            if (a1->frequency == a2->frequency) {
                int dx = a2->column - a1->column;
                int dy = a2->row - a1->row;
                int x1 = a1->column - dx;
                int y1 = a1->row - dy;
                int x2 = a2->column + dx;
                int y2 = a2->row + dy;

                if (x1 >= 0 && x1 < columns && y1 >= 0 && y1 < rows) {
                    Interference* new_item = malloc(sizeof(Interference));
                    if (!new_item) {
                        free_interference(head);
                        return NULL;
                    }
                    new_item->row = y1;
                    new_item->column = x1;
                    new_item->next = head;
                    head = new_item;
                }
                if (x2 >= 0 && x2 < columns && y2 >= 0 && y2 < rows) {
                    Interference* new_item = malloc(sizeof(Interference));
                    if (!new_item) {
                        free_interference(head);
                        return NULL;
                    }
                    new_item->row = y2;
                    new_item->column = x2;
                    new_item->next = head;
                    head = new_item;
                }
            }
        }
    }
    return head;
}

bool print_interference(Interference* list) {
    if (!list) return false;
    while (list) {
        printf("Interference at (%d, %d)\n", list->row, list->column);
        list = list->next;
    }
    return true;
}

bool free_interference(Interference* list) {
    if (!list) return false;
    while (list) {
        Interference* temporary = list;
        list = list->next;
        free(temporary);
    }
    return true;
}

bool free_graph(Vertex* list) {
    bool freed = false;

    while (list) {
        Adjacency* a = list->adjacent;
        while (a) {
            Adjacency* temp_edge = a;
            a = a->next;
            free(temp_edge);
        }

        Vertex* temp_vertex = list;
        list = list->next;
        free(temp_vertex);
        freed = true;
    }

    return freed;
}

bool segment_intersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, double* ix, double* iy)
{
    
    int dx1 = x2 - x1;
    int dy1 = y2 - y1;
    int dx2 = x4 - x3;
    int dy2 = y4 - y3;

    int det = dx1 * (-dy2) - dy1 * (-dx2);
    if (det == 0) {
        
        return false;
    }

    int rx = x3 - x1;
    int ry = y3 - y1;
    
     double t = (double)( rx * (-dy2) - ry * (-dx2) ) / det;
    double u = (double)( dx1 * ry - dy1 * rx) / det;

    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
        if (ix) *ix = x1 + t * dx1;
        if (iy) *iy = y1 + t * dy1;
        return true;
    }

    return false;
}

bool print_intersections(Vertex* list) {
    bool found = false;

    for (Vertex* v1 = list; v1 != NULL; v1 = v1->next) {
        for (Adjacency* a1 = v1->adjacent; a1 != NULL; a1 = a1->next) {
            int x1 = v1->row;
            int y1 = v1->column;
            int x2 = a1->row;
            int y2 = a1->column;

            for (Vertex* v2 = v1; v2 != NULL; v2 = v2->next) {
                for (Adjacency* a2 = (v1 == v2) ? a1->next : v2->adjacent; a2 != NULL; a2 = a2->next) {
                    int x3 = v2->row;
                    int y3 = v2->column;
                    int x4 = a2->row;
                    int y4 = a2->column;

                    if ((x1 == x3 && y1 == y3 && x2 == x4 && y2 == y4) ||
                        (x1 == x4 && y1 == y4 && x2 == x3 && y2 == y3)) {
                        continue;
                    }

                    double ix, iy;
                    if (segment_intersection(x1, y1, x2, y2, x3, y3, x4, y4, &ix, &iy)) {
                        printf("Intersection at (%.2lf,%.2lf) between (%d,%d)->(%d,%d) and (%d,%d)->(%d,%d)\n",
                               ix, iy, x1, y1, x2, y2, x3, y3, x4, y4);
                        found = true;
                    }
                }
            }
        }
    }

    if (!found) {
        printf("No intersections found.\n");
    }

    return found;
}

bool connect_same_frequency(Vertex* list) {
    if (!list) return false;

    bool connected = false;

    for (Vertex* v1 = list; v1; v1 = v1->next) {
        for (Vertex* v2 = v1->next; v2; v2 = v2->next) {
            if (v1->frequency == v2->frequency) {
                
                Adjacency* a1 = create_adjacency(v2->row, v2->column, v2->frequency);
                if (!a1) return false;
                v1->adjacent = insert_adjacency(v1->adjacent, a1);

                Adjacency* a2 = create_adjacency(v1->row, v1->column, v1->frequency);
                if (!a2) return false;
                v2->adjacent = insert_adjacency(v2->adjacent, a2);

                connected = true;
            }
        }
    }

    return connected;
}
