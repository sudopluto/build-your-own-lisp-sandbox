all: run

run: sudolisp
	- ./sudolisp 

test: sudolisp
	./sudolisp < ./test_files/in_test_atomics.txt > ./test_files/out_test_atomics.txt
	diff -q ./test_files/out_test_atomics.txt ./test_files/expect_test_atomics.txt && printf '\n\nTEST PASSED!!\n'

dbg: sudolisp
	- gdb ./sudolisp 

dbg-mem: sudolisp
	- valgrind -s --leak-check=full --show-leak-kinds=all ./sudolisp

sudolisp: ./sudolisp.cpp console types parser eval
	g++ -Wall -g -o sudolisp sudolisp.cpp ./console/console.o ./parser/mpc/mpc.o

console: ./console/console.o

console/console.o: ./console/console.h ./console/console.cpp
	g++ -Wall -g -c -o ./console/console.o ./console/console.cpp

clean-console:
	- rm ./console/*.o

types: ./types/types.h

parser: ./parser/mpc/mpc.o ./parser/parser.h

parser/mpc/mpc.o: ./parser/mpc/mpc.h ./parser/mpc/mpc.c
	gcc -std=c99 -Wall -g -c -o ./parser/mpc/mpc.o ./parser/mpc/mpc.c

clean-parser:
	- rm ./parser/*.o
	- rm ./parser/mpc/*.o

eval: ./evaluator/eval.h

clean: clean-console clean-parser
	- rm ./sudolisp
	- rm ./test_files/out*.txt
