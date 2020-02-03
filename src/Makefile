all: run

run: sudolisp
	- ./sudolisp 

dbg: sudolisp
	- gdb ./sudolisp 

sudolisp: sudolisp.cpp
	g++ -g -Wall -o sudolisp sudolisp.cpp -ledit

clean:
	rm sudolisp
