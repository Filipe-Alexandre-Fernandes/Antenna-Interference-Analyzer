# Latest archive integration

Source: `Filipe Fernandes_a31498(1).zip`, supplied by the author on 4 November 2026. The top-level folder contains eight C/header files, a sample matrix, Doxygen configuration, two PDFs and prebuilt object/library files. Generated documentation is also included in the archive.

## File mapping

| Archive file | Repository path |
| --- | --- |
| `main.c` | `graph/main.c` |
| `grafo_structs.h` | `graph/graph_types.h` |
| `grafo_funcoes.c`, `grafo_funcoes.h` | `graph/graph.c`, `graph/graph.h` |
| `ficheiros.c`, `ficheiros.h` | `graph/graph_io.c`, `graph/graph_io.h` |
| `procura.c`, `procura.h` | `graph/search.c`, `graph/search.h` |
| `matriz.txt` | `graph/matrix.txt` (unchanged bytes) |
| `Relatorio-EDA-final-filipe.pdf` | `docs/project-report.pdf` (unchanged bytes) |
| `Relatorio-Doxygen.pdf` | `docs/doxygen-report.pdf` (unchanged bytes) |
| `Doxyfile` | Replaced with a concise root configuration for the English source |

Precompiled `.o`/`.a` files and generated HTML/LaTeX are excluded. The library and documentation can be regenerated from the current sources. Phase 1 remains at the repository root, with the recovered Phase 2 isolated in `graph/` so each demo builds independently.

## Original-to-English API names

| Original | English |
| --- | --- |
| `Vertice`, `Adjacencia`, `Interferencia` | `Vertex`, `Adjacency`, `Interference` |
| `criarVertice`, `inserirVertice`, `encontrarVertice`, `contarVertices` | `create_vertex`, `insert_vertex`, `find_vertex`, `count_vertices` |
| `criarAdjacente`, `inserirAdjacente` | `create_adjacency`, `insert_adjacency` |
| `listarAntenas`, `ligarAntenasMesmaFrequencia` | `list_antennas`, `connect_same_frequency` |
| `calcularInterferencias`, `imprimirInterferencias` | `calculate_interference`, `print_interference` |
| `intersecoesParametricas`, `detetarIntersecoes` | `segment_intersection`, `print_intersections` |
| `libertarInterferencias`, `libertarGrafo` | `free_interference`, `free_graph` |
| `carregarMatriz` | `load_matrix` |
| `gravarGrafoBincomInterferencias`, `lerGrafoBinario` | `save_graph_binary`, `load_graph_binary` |
| `dfs`, `bfs` | Unchanged |

## Integration corrections

- Removed an unmatched comment terminator in the graph structure header that prevented compilation.
- Translated filenames, identifiers, console messages and documentation, and added a search-header include guard.
- Added Makefiles for the executable/static library and automated regression checks.
- Fixed BFS to mark vertices when enqueued, preventing duplicate queue entries from overflowing its vertex-sized buffer. Added allocation and traversal-size checks.
- Corrected map width for a final line without a newline, stripped CRLF and rejected blank/oversized rows instead of splitting them into extra rows.
- Added allocation checks for interference nodes and read/write checks for native graph persistence; failed vertex/edge reads release partial results.
- Removed the duplicate “no intersections” message in the demo. The original intersection comparison algorithm, including repeated crossing output, remains unchanged.

## Report verification

The report inside this ZIP has the same SHA-256 as the previously supplied `Relatorio-EDA-final.pdf`. The latest ZIP's file is the authoritative copy at the existing report link; Git does not show a binary change because the content is identical.

| Document | SHA-256 |
| --- | --- |
| Final report | `c1d44b0e6e33ccebf849592ddf034c42cb6d8cd98e094bf9e4838e580b111d3d` |
| Doxygen report | `f05723a154b03d0b907188627f3b444bf1d8138d7cdf60c25ee3c2dea0aa0e65` |

The PDFs preserve the original academic submission. They have not been edited to imply that subsequent integration fixes were part of that submission.
