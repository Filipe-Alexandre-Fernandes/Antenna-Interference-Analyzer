all: prog

FuncoesAntenas.o: FuncoesAntenas.c FuncoesAntenas.h estruturas.h
	gcc -c FuncoesAntenas.c

prog: FuncoesAntenas.o main.c
	gcc main.c FuncoesAntenas.o -o prog

run:
	./prog

clean:
	rm -f *.o prog
