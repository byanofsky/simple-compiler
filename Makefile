CC=gcc
CFLAGS=-Wall -g

all: generate_llvm_ir

printer.o: printer.c printer.h
	$(CC) $(CFLAGS) -c $< -o $@

lexer-v1.o: lexer-v1.c lexer-v1.h
	$(CC) $(CFLAGS) -c $< -o $@

parser-v1.o: parser-v1.c parser-v1.h lexer-v1.o
	$(CC) $(CFLAGS) -c $< -o $@

generate_llvm_ir: generate_llvm_ir.c parser-v1.o lexer-v1.o
	$(CC) $(CFLAGS) $^ -o $@ 

compiler: compiler.c generate_llvm_ir
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f generate_llvm_ir compiler *.o test-programs-out/*

test: compile_test_program
	./test-programs-out/test-program-1 ./test-programs-out/test-program-parse-error

test_program_1: generate_llvm_ir
	./generate_llvm_ir < ./test-programs/test-program-1.txt > ./test-programs-out/test-program-1.ll

compile_test_program: test_program_1
	clang ./test-programs-out/test-program-1.ll ./print_integer.c -o ./test-programs-out/test-program-1

test_program-parse-error: generate_llvm_ir
	./generate_llvm_ir < ./test-programs/test-program-parse-error.txt > ./test-programs-out/test-program-parse-error.ll

compile_test_program_parse_error: test_program_parse_error
	clang ./test-programs-out/test-program-parse-error.ll ./print_integer.c -o ./test-programs-out/test-program-parse-error

