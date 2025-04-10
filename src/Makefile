all: run

run: sudolisp
	- ./sudolisp 

dbg: sudolisp
	- gdb ./sudolisp 

dbg-mem: sudolisp
	- valgrind -s --leak-check=full --show-leak-kinds=all ./sudolisp

sudolisp: sudolisp.cpp parser.h eval.h types.h console.o mpc.o
	g++ -Wall -g -o sudolisp sudolisp.cpp mpc.o console.o

mpc.o: ./mpc/mpc.h ./mpc/mpc.c
	gcc -std=c99 -Wall -g -c -o mpc.o ./mpc/mpc.c

console.o: ./console/console.h ./console/console.cpp
	g++ -Wall -g -c -o console.o ./console/console.cpp

clean:
	- rm sudolisp
	- rm mpc.o
	- rm console.o
