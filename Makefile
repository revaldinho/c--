SUFFIXES:

SUFFIXES: .c .o
.c.o:;gcc -O -c $< 

all:	Compiler

Compiler: Compiler.o analyse.o code.o symboltable.o
	gcc -o Compiler Compiler.o analyse.o code.o symboltable.o

clean:
	rm -rf *.o Compiler
