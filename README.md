# Antenna Interference Analyzer

A C11 console project that loads antenna maps into a singly linked list and displays interference locations produced by pairs of antennas with the same frequency.

Developed by **Filipe Fernandes (31498)** for **Advanced Data Structures (EDA)** at IPCA.

## Scope

This repository currently contains the **Phase 1 linked-list implementation**. The final report and assignment also describe **Phase 2 graph algorithms**, but their source files are not present in this repository. DFS, BFS, path enumeration and segment intersections are therefore not available in this version.

## Features

- Load an antenna map from a text file.
- Create, search, insert in coordinate order and remove antennas through the C API.
- Print antenna frequencies and one-based `(row, column)` coordinates.
- Render same-frequency interference locations as `#` on a grid.
- Export antennas and interference cells as native binary records.

The executable runs a fixed demonstration pipeline. Insertion and removal functions are available in the API, not through an interactive menu.

## Build and run

Requirements: GCC (or a compatible C compiler with variable-length array support) and Make. Linux, macOS with appropriate command-line tools, or WSL are suitable environments.

From the repository root:

```sh
make
make run
```

The program reads `antennas.txt` from the current working directory and writes `antennas.bin` there. The supplied map has **9 rows, 12 columns and 6 antennas**. To try a different map, edit the input file or run the executable from a directory containing another `antennas.txt`.

```sh
make clean
```

This removes the executable, object files and generated `antennas.bin`.

## Input and coordinates

Each line represents a map row. `.` is an empty cell; other characters represent antenna frequencies. Frequency matching is case-sensitive. Use plain text with LF line endings and no blank rows. The current reader uses a fixed 128-byte buffer, so keep rows to **126 cells or fewer** for newline-terminated input.

The data structure stores `x = column` and `y = row`, both starting at 1. Console coordinates are printed as `(row, column)`.

For two equal-frequency antennas at positions **A** and **B**, the algorithm checks **2A − B** and **2B − A**. It marks in-bounds empty cells with `#`. Different frequencies do not interact.

## Source layout

| File | Purpose |
| --- | --- |
| [main.c](main.c) | Load, list, render and export demonstration |
| [antennas.c](antennas.c) | Linked-list operations, input and interference logic |
| [antennas.h](antennas.h) | Public function declarations |
| [antenna_types.h](antenna_types.h) | Antenna and binary record structures |
| [antennas.txt](antennas.txt) | Original example map |
| [tests/smoke_test.py](tests/smoke_test.py) | Executable behavior checks |
| [docs/project-guide.md](docs/project-guide.md) | English overview of the assignment and report |
| [docs/project-report.pdf](docs/project-report.pdf) | Original final report, in Portuguese |
| [docs/assignment.pdf](docs/assignment.pdf) | Original assignment, in Portuguese |

Source filenames, identifiers, comments and console messages are in English. The original PDFs are retained unchanged as academic source documents; the English guide is a summary, not a full translation.

## Validation

```sh
make
python3 tests/smoke_test.py
```

The English version was built with GCC using `-std=c11 -Wall -Wextra -Wpedantic` without warnings. Its supplied-map grid and decoded binary records were compared with the original implementation. Smoke tests cover the supplied map, equal-frequency interference, distinct frequencies and empty input.

## Known limitations

- The assignment allows interference on occupied antenna locations; this implementation only marks empty cells and preserves antenna characters.
- Interference is rendered in a temporary grid, not stored as a separate linked list as requested in the assignment.
- Binary export derives bounds from antenna positions, so it may omit interference in empty outer margins of the original map.
- Binary output writes native C structures, including platform-dependent layout; it is not a portable interchange format.
- The original list API does not safely handle every duplicate-coordinate case, and the demo does not free the full list before exit.
- Very large maps are unsuitable for the fixed line buffer and stack-allocated grids. File-write failures are not fully checked.

The translation preserves the existing algorithm. These limitations are documented rather than presented as completed requirements.

## Next steps

- Recover the Phase 2 graph implementation from the final project archive.
- Improve input validation, memory ownership and binary error handling.
- Represent interference independently from antenna display and preserve full map bounds during export.
- Add portable persistence and broader automated tests.

## License

No license has been selected. The supplied academic documents retain their original authorship.
