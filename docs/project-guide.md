# Project guide

## Problem and assignment

The assignment represents a city as a character grid. Each antenna has coordinates and a frequency. For two antennas with the same frequency, interference candidates lie at twice one position minus the other. Candidates outside the map are discarded.

Phase 1 requests singly linked lists, file loading, insertion/removal, a separate interference list and tabular output. Phase 2 requests graphs connecting equal-frequency antennas, DFS, BFS, all paths between two antennas, and segment intersections between two frequency groups.

## Implementation coverage

| Requirement | Included implementation |
| --- | --- |
| Antenna linked list and editing | Phase 1 API; fixed demo in the executable |
| Separate interference list | Phase 2; Phase 1 only renders a grid |
| Graph and equal-frequency connections | Phase 2; both directions stored in adjacency lists |
| DFS and BFS | Phase 2 library API and regression tests |
| All paths between two antennas | Not implemented in the supplied source |
| Segment intersection | Parametric segment test and printed graph intersections; no frequency-group selection or result deduplication |
| File persistence | Both phases have native binary output; Phase 2 can reload vertices and edges |
| Arbitrary map dimensions | Not fully supported; fixed line buffers and algorithm limits remain |

The supplied report discusses both phases and illustrates the parametric intersection equations, DFS and BFS. The repository now contains the graph modules described in that report. This does not imply that every assignment requirement is complete.

## Using the graph library

Run `make graph` from the repository root to build `graph/libantenna_graph.a`. Include the headers from `graph/` and link against the archive. This example visits the connected component of the first loaded vertex:

```c
#include <stdlib.h>
#include "graph.h"
#include "graph_io.h"
#include "search.h"

int main(void) {
    int rows = 0, columns = 0;
    Vertex* graph = load_matrix("matrix.txt", &rows, &columns);
    if (!graph) return 1;
    connect_same_frequency(graph);

    int total = count_vertices(graph);
    bool* visited = calloc((size_t)total, sizeof(*visited));
    if (!visited) {
        free_graph(graph);
        return 1;
    }
    dfs(graph, graph->row, graph->column, visited, total);
    bfs(graph, graph->row, graph->column, total);
    free(visited);
    free_graph(graph);
    return 0;
}
```

With the example saved as `example.c`, compile from the root using:

```sh
cc -std=c11 -Igraph example.c graph/libantenna_graph.a -o example
```

Run it from a directory containing `matrix.txt`. Coordinates are zero-based. Vertices are prepended on load, so traversal indexing follows the reverse of the map's row-major antenna order. The DFS caller owns the zeroed visited array, which must contain at least `count_vertices(graph)` entries. BFS allocates its own queue and visited array. Both traverse only the origin's connected component.

`connect_same_frequency` returns false both when there are no matching pairs and on allocation failure; it is not a transactional operation. Call it once on an unconnected graph.

## Persistence and ownership

`calculate_interference` returns a list owned by the caller. Release it with `free_interference`, and release all graph vertices/edges with `free_graph`.

`save_graph_binary` writes a native integer vertex count, then each vertex's row, column, character frequency and adjacency count, followed by its adjacency records. An interference count and row/column pairs follow. Integers have the host's size and byte order.

`load_graph_binary` requires an initially NULL destination. It restores vertices and adjacency records, checks reads/allocation and cleans up on failure. It does not load or validate the interference trailer. These files are not portable interchange files; the Phase 1 and Phase 2 formats are different.

## Academic source documents

- [Final project report](project-report.pdf), taken from `Relatorio-EDA-final-filipe.pdf` in the latest ZIP. Its cover is dated April 2025 and it contains both project phases.
- [Doxygen report](doxygen-report.pdf), taken from `Relatorio-Doxygen.pdf` in the same ZIP. It describes the original Portuguese API, before this repository's translation and corrections.
- [Assignment](assignment.pdf), the previously supplied course requirements.

All PDFs are kept in their original Portuguese. See the [archive notes](archive-notes.md) for the exact file mapping and changes made during integration.
