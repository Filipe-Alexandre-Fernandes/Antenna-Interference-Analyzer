/** @file graph_types.h
 * @brief Antenna graph project, adapted from Filipe Fernandes' May 2025 submission.
 */
#ifndef GRAPH_TYPES_H
#define GRAPH_TYPES_H

typedef struct Adjacency {
    int row, column;
    char frequency;
    struct Adjacency* next;
} Adjacency;

typedef struct Vertex {
    int row, column;
    char frequency;
    Adjacency* adjacent;
    struct Vertex* next;
} Vertex;

typedef struct Interference {
    int row;
    int column;
    struct Interference* next;
} Interference;

#endif
