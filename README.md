# Antenna Interference Analyzer

A C11 project for analyzing antenna maps with **linked lists and graphs**. It finds interference locations between equal-frequency antennas, builds adjacency lists, supports DFS/BFS traversal and calculates intersections between antenna connections.

Developed by **Filipe Fernandes (31498)** for **Advanced Data Structures (EDA)** at IPCA.

## Two project phases

| | Phase 1: linked lists | Phase 2: graphs |
| --- | --- | --- |
| Location | Repository root | [`graph/`](graph/) |
| Data structures | Ordered singly linked antenna list | Vertex list, adjacency lists and interference list |
| Demo | Load, list, display interference and export binary records | Load, connect equal frequencies, list interference and segment intersections |
| Additional API | Find, insert and remove antennas | DFS, BFS and native binary persistence |
| Coordinates | One-based `(row, column)` | Zero-based `(row, column)` |
| Example map | 9 × 12, 6 antennas | 12 × 12, 4 antennas |

Both source implementations are included. The graph phase was recovered from the latest supplied project archive. **All-path enumeration is not implemented**, and the intersection routine does not yet restrict comparisons to two selected frequency groups.

## Build and run

Requirements: GCC (or a compatible C11 compiler with variable-length array support), Make and `ar`. Python 3 is needed for tests. Linux, macOS with command-line build tools, or WSL are suitable environments.

Run these commands from the repository root:

```sh
# Phase 1
make
make run

# Phase 2: executable and static library
make graph
make run-graph

# Regression checks for both phases
make test

# Remove compiled/generated program files
make clean
```

Phase 1 reads `antennas.txt` and writes `antennas.bin` in its working directory. `make run-graph` runs inside `graph/`, where the graph executable reads `matrix.txt`. The graph demo prints results but does not call the DFS/BFS or binary APIs; those are available in `libantenna_graph.a` and exercised by the tests. Neither demo provides an interactive menu.

## Input and interference rule

Each text line is a map row. `.` is empty; any other character represents an antenna frequency. Matching is case-sensitive. For equal-frequency antennas at **A** and **B**, the algorithm checks **2A − B** and **2B − A**, retaining positions inside the map bounds.

- **Phase 1:** use LF text, no blank rows and at most 126 cells per row. The grid displays interference as `#` only on empty cells.
- **Phase 2:** accepts LF/CRLF and a final line without a newline. Rows must have 1–253 cells; blank and oversized rows are rejected. Interference is a separate list and may overlap antennas or contain duplicate positions. Ragged rows use the longest row as the map width.

The graph example contains two `A` and two `C` antennas. Their segments cross at approximately **(row 5.33, column 4.86)**. No interference candidates fall within this example's map. The original intersection routine prints that crossing four times because each undirected connection is stored in both directions.

## Source and documentation

| Path | Purpose |
| --- | --- |
| [main.c](main.c), [antennas.c](antennas.c), [antennas.h](antennas.h), [antenna_types.h](antenna_types.h) | Phase 1 demo, linked-list API and types |
| [graph/main.c](graph/main.c) | Graph demonstration |
| [graph/graph.c](graph/graph.c), [graph/graph.h](graph/graph.h), [graph/graph_types.h](graph/graph_types.h) | Graph storage, interference, intersections and cleanup |
| [graph/graph_io.c](graph/graph_io.c), [graph/graph_io.h](graph/graph_io.h) | Map loading and binary persistence |
| [graph/search.c](graph/search.c), [graph/search.h](graph/search.h) | DFS and BFS |
| [tests/](tests/) | Executable and graph API regression checks |
| [docs/project-guide.md](docs/project-guide.md) | English guide, API usage and assignment coverage |
| [docs/archive-notes.md](docs/archive-notes.md) | Archive provenance, filename mapping and integration fixes |
| [docs/project-report.pdf](docs/project-report.pdf) | Final report from the latest ZIP, in Portuguese |
| [docs/doxygen-report.pdf](docs/doxygen-report.pdf) | Original generated API report from that ZIP, in Portuguese |
| [docs/assignment.pdf](docs/assignment.pdf) | Original assignment, in Portuguese |

Source filenames, identifiers, comments and console messages are in English. The original PDFs remain unchanged; the English guide is not a full translation. The Doxygen PDF documents the original Portuguese API names, mapped in the archive notes.

To generate HTML documentation from the current English source, install Doxygen and run `doxygen Doxyfile` from the root. Output goes to `docs/generated/html/` and is ignored by Git.

## Validation

Both phases compile with `-std=c11 -Wall -Wextra -Wpedantic` without warnings. `make test` checks the Phase 1 sample grid and interference behavior, plus graph connections, DFS/BFS, interference bounds and occupied cells, segment geometry, binary vertex/edge round trips, truncated vertex data, CRLF, missing files, empty maps and oversized rows.

Graph regression checks also passed with AddressSanitizer and UndefinedBehaviorSanitizer. Leak detection was disabled because the validation environment does not support LeakSanitizer. Doxygen generation was not run in that environment.

## Remaining limitations

- Phase 1 preserves the earlier implementation: interference exists only in a temporary grid, binary bounds derive from occupied cells, duplicate-coordinate insertion has unsafe cases, and the demo does not release the antenna list on exit.
- Graph interference entries and segment crossings are not deduplicated. Intersections include endpoint contacts and same-frequency pairs; parallel/collinear segments return no intersection.
- DFS is recursive; dense graphs and large inputs are unsuitable for the current algorithms. Allocation-failure results are not always distinguishable from empty results, and graph connection can leave partial edges on allocation failure.
- The graph connector is intended to run once per freshly loaded graph. Repeating it adds duplicate adjacency records.
- Both binary formats use native C types and are platform-dependent. The graph reader restores vertices and edges only; it intentionally ignores the interference trailer and does not validate the graph's semantic consistency.
- Phase 1 file-write errors are not fully checked. No all-path enumeration or interactive graph operations are included.

## License

No license has been selected. The supplied academic documents retain their original authorship.
