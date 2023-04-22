CC=gcc
CFLAGS=-Wall -g

all: generate_llvm_ir

generate_llvm_ir: generate_llvm_ir.c parser-v1.o
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -f generate_llvm_ir *.o

test: compile_test_program
	./test-programs/test-program-1

test_program_1: generate_llvm_ir
	./generate_llvm_ir < ./test-programs/test-program-1.txt > ./test-programs/test-program-1.ll

compile_test_program: test_program_1
	clang ./test-programs/test-program-1.ll ./test-programs/print_integer.c -o ./test-programs/test-program-1