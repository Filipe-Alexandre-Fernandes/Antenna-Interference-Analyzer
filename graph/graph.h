/** @file graph.h
 * @brief Antenna graph project, adapted from Filipe Fernandes' May 2025 submission.
 */
#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "graph_types.h"

/** @brief Allocate an isolated vertex; return NULL on allocation failure. */
Vertex* create_vertex(int row, int column, char frequency);

/** @brief Prepend an allocated vertex and return the new list head. */
Vertex* insert_vertex(Vertex* list, Vertex* new_vertex);

/** @brief Find the vertex at zero-based (row, column), or return NULL. */
Vertex* find_vertex(Vertex* list, int row, int column);

/** @brief Count all vertices in the list. */
int count_vertices(Vertex* list);

/** @brief Allocate an adjacency record; return NULL on allocation failure. */
Adjacency* create_adjacency(int row, int column, char frequency);

/** @brief Prepend an allocated adjacency record. */
Adjacency* insert_adjacency(Adjacency* adjacency_list, Adjacency* new_item);

/** @brief Print antenna coordinates and frequencies; return false for an empty list. */
bool list_antennas(Vertex* list);

/** @brief Return a caller-owned interference list, or NULL for no results/allocation failure. Duplicates are retained. */
Interference* calculate_interference(Vertex* list, int rows, int columns);

/** @brief Print interference coordinates; return false for an empty list. */
bool print_interference(Interference* list);

/** @brief Free the interference list; return whether any node was freed. */
bool free_interference(Interference* list);

/** @brief Free all vertices and adjacency records; return whether any vertex was freed. */
bool free_graph(Vertex* list);

/** @brief Intersect closed segments parametrically; parallel/collinear segments return false. */
bool segment_intersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, double* ix, double* iy);

/** @brief Print intersections across stored directed edges; duplicate locations may be printed. */
bool print_intersections(Vertex* list);

/** @brief Add both directions for each equal-frequency pair. Call once on an unconnected graph. False means no links or allocation failure. */
bool connect_same_frequency(Vertex* list);

#endif
