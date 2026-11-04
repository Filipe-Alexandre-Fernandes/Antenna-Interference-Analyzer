#ifndef ANTENNA_SEARCH_H
#define ANTENNA_SEARCH_H
/** @file search.h
 * @brief Antenna graph project, adapted from Filipe Fernandes' May 2025 submission.
 */
#include "graph.h"

/** @brief Visit the origin component recursively. Caller supplies a zeroed visited array with total entries in vertex-list order; total must cover every vertex. */
bool dfs(Vertex* list, int row, int column, bool* visited, int total);

/** @brief Visit the origin component with a queue; total must cover every vertex. Return false for an invalid origin, size or allocation failure. */
bool bfs(Vertex* list, int row, int column, int total);

#endif
