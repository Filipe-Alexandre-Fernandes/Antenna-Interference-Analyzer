# Project guide

## Assignment overview

The assignment models a city as a character grid. Each antenna has a frequency and coordinates. Antennas with equal frequencies create interference at positions aligned with the pair, where one antenna is twice as far from the position as the other. Different frequencies do not create these positions together.

Phase 1 requires a singly linked list of antennas, text-file loading, insertion/removal, automatic interference detection and tabular output. It also requests interference locations as a linked list and support for maps of arbitrary dimensions.

Phase 2 requires a graph with one antenna per vertex and edges between antennas of equal frequency. Operations include depth-first and breadth-first traversal, enumeration of all paths between two antennas, and intersections between segments belonging to two different frequency groups.

## Final report overview

The report presents the academic context, linked-list structures, file handling, interference calculations and example results. Its second phase discusses graph representation, segment intersection through parametric equations, DFS and BFS.

Only the linked-list source is currently available on GitHub. The graph modules described in the report (including graph structures, graph functions, file handling and search modules) must be recovered from the final source archive before Phase 2 can be documented as runnable.

## Original documents

- [Final report](project-report.pdf): Filipe Alexandre Coelho Fernandes, April 2025.
- [Assignment](assignment.pdf): original course requirements, including both phases.

Both PDFs are preserved in Portuguese. This English guide summarizes their project requirements and scope; it does not replace the original documents or claim all requirements are implemented.
