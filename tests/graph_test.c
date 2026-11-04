/* Regression checks for the restored graph API. Run in a temporary directory. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "graph_io.h"
#include "search.h"

static void write_map(const char* content) {
    FILE* f = fopen("map.txt", "wb");
    assert(f);
    assert(fwrite(content, 1, strlen(content), f) == strlen(content));
    assert(fclose(f) == 0);
}

int main(void) {
    int rows, columns;
    /* Four connected vertices exposed the original BFS queue overflow. */
    write_map("AAAA.B\r\n......\r\n");
    Vertex* graph = load_matrix("map.txt", &rows, &columns);
    assert(graph && rows == 2 && columns == 6 && count_vertices(graph) == 5);
    assert(connect_same_frequency(graph));
    for (Vertex* v = graph; v; v = v->next) {
        int edges = 0;
        for (Adjacency* a = v->adjacent; a; a = a->next) {
            assert(a->frequency == v->frequency);
            assert(find_vertex(graph, a->row, a->column));
            edges++;
        }
        assert(edges == (v->frequency == 'A' ? 3 : 0));
    }
    bool visited[5] = {false};
    assert(dfs(graph, 0, 0, visited, 5));
    int seen = 0;
    for (int i = 0; i < 5; i++) seen += visited[i];
    assert(seen == 4 && !visited[0]);
    assert(bfs(graph, 0, 0, 5));
    assert(!bfs(graph, 0, 0, 1));
    assert(!dfs(graph, 0, 0, NULL, 5));
    assert(!bfs(graph, 10, 10, 5));

    Interference* interference = calculate_interference(graph, rows, columns);
    assert(interference);
    bool occupied = false;
    for (Interference* p = interference; p; p = p->next) {
        assert(p->row == 0 && p->column >= 0 && p->column < columns);
        if (find_vertex(graph, p->row, p->column)) occupied = true;
    }
    assert(occupied);
    assert(save_graph_binary("graph.bin", graph, interference));
    Vertex* restored = NULL;
    assert(load_graph_binary("graph.bin", &restored));
    assert(count_vertices(restored) == 5);
    for (Vertex* v = graph; v; v = v->next) {
        Vertex* copy = find_vertex(restored, v->row, v->column);
        assert(copy && copy->frequency == v->frequency);
        int original_edges = 0, copied_edges = 0;
        for (Adjacency* a = v->adjacent; a; a = a->next) original_edges++;
        for (Adjacency* a = copy->adjacent; a; a = a->next) copied_edges++;
        assert(original_edges == copied_edges);
    }
    free_graph(restored);
    restored = NULL;
    FILE* broken = fopen("broken.bin", "wb");
    assert(broken);
    int count = 1;
    assert(fwrite(&count, sizeof(count), 1, broken) == 1);
    fclose(broken);
    assert(!load_graph_binary("broken.bin", &restored) && !restored);
    free_interference(interference);
    free_graph(graph);

    double x, y;
    assert(segment_intersection(0, 0, 2, 2, 0, 2, 2, 0, &x, &y));
    assert(x == 1.0 && y == 1.0);
    assert(!segment_intersection(0, 0, 2, 0, 0, 1, 2, 1, &x, &y));
    assert(!segment_intersection(0, 0, 1, 0, 2, -1, 2, 1, &x, &y));

    /* A final line without LF must retain its last column in the bounds. */
    write_map(".A.A.#");
    graph = load_matrix("map.txt", &rows, &columns);
    assert(rows == 1 && columns == 6);
    interference = calculate_interference(graph, rows, columns);
    assert(interference && interference->column == 5);
    free_interference(interference);
    free_graph(graph);
    write_map("...");
    assert(!load_matrix("map.txt", &rows, &columns));
    assert(rows == 1 && columns == 3);
    puts("PASS: graph API regression checks");
    return 0;
}
