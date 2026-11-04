/** @file main.c
 * @brief Antenna graph project, adapted from Filipe Fernandes' May 2025 submission.
 */
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "graph_io.h"

int main(void) {
    Vertex* graph = NULL;
    Interference* interference = NULL;
    int rows = 0, columns = 0;

    graph = load_matrix("matrix.txt", &rows, &columns);
    if (!graph) {
        fprintf(stderr, "Could not load the antenna map.\n");
        return 1;
    }
    connect_same_frequency(graph);

    printf("\n--- Loaded antennas ---\n");
    list_antennas(graph);

    printf("\n--- Interference locations ---\n");
    interference = calculate_interference(graph, rows, columns);
    print_interference(interference);

    printf("\n--- Edge intersections ---\n");
    print_intersections(graph);

    free_interference(interference);
    free_graph(graph);

    return 0;
}
