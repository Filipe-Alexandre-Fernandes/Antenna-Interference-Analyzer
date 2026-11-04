CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = antenna-analyzer

.PHONY: all run clean
all: $(TARGET)

$(TARGET): main.o antennas.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

main.o antennas.o: antennas.h antenna_types.h

run: $(TARGET)
	./$(TARGET)

clean:
	$(MAKE) -C graph clean
	rm -f *.o $(TARGET) antennas.bin

.PHONY: graph run-graph test
graph:
	$(MAKE) -C graph
run-graph:
	$(MAKE) -C graph run
test: all
	python3 tests/smoke_test.py
	$(MAKE) -C graph test
