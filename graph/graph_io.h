/** @file graph_io.h
 * @brief Antenna graph project, adapted from Filipe Fernandes' May 2025 submission.
 */
#ifndef GRAPH_IO_H
#define GRAPH_IO_H

#include <stdbool.h>
#include "graph_types.h"

/** @brief Load 1–253 cells per row (LF/CRLF), returning NULL on failure or no antennas. Output coordinates are zero-based. */
Vertex* load_matrix(const char* filename, int* rows, int* columns);

/** @brief Write native integer/character fields for vertices, edges and interference; not a portable format. */
bool save_graph_binary(const char* filename, Vertex* list, Interference* interference);

/** @brief Load vertices and edges only; ignore the interference trailer. Caller must pass an empty list (*list == NULL). Failures leave it empty. */
bool load_graph_binary(const char* filename, Vertex** list);

#endif
