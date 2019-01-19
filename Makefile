CC=gcc

CFLAGS=-g -O1 -std=c99 -pthread -lm -Wall -Wextra 

all: run_aluno grade clean

memvirt.o: memvirt.c

aluno.o: aluno.c

test.o: test.c

aluno: memvirt.o aluno.o 
	$(CC) $(CFLAGS) memvirt.o aluno.o -o aluno -lm

test: memvirt.o test.o
	$(CC) $(CFLAGS) memvirt.o test.o -o test -lm

run_aluno: aluno
	./aluno

grade: test
	./test

clean:
	rm -rf *.o test aluno
