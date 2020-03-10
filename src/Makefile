all: run

run: sudolisp
	- ./sudolisp 

dbg: sudolisp
	- gdb ./sudolisp 

sudolisp: sudolisp.cpp impl.h console-io.h mpc.o
	g++ -g -Wall -o sudolisp sudolisp.cpp mpc.o -ledit

mpc.o: ./mpc/mpc.h ./mpc/mpc.c
	gcc -std=c99 -Wall -g -c -o mpc.o ./mpc/mpc.c

clean:
	- rm sudolisp
	- rm mpc.o
