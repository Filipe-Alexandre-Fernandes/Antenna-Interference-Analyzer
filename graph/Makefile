CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = antenna-graph
OBJECTS = graph.o graph_io.o search.o

.PHONY: all run test clean
all: $(TARGET) libantenna_graph.a
$(TARGET): main.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
libantenna_graph.a: $(OBJECTS)
	$(AR) rcs $@ $^
main.o $(OBJECTS): graph.h graph_types.h graph_io.h search.h
graph-tests: ../tests/graph_test.c $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -I. $^ $(LDLIBS) -o $@
run: $(TARGET)
	./$(TARGET)
test: $(TARGET) graph-tests
	python3 ../tests/graph_smoke_test.py
clean:
	rm -f *.o *.a $(TARGET) graph-tests
