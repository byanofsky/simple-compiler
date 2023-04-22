CC=gcc
CFLAGS=-Wall -g

all: generate_llvm_ir

lexer-v1.o: lexer-v1.c lexer-v1.h
	$(CC) $(CFLAGS) -c $< -o $@

parser-v1.o: parser-v1.c parser-v1.h lexer-v1.o
	$(CC) $(CFLAGS) -c $< -o $@

generate_llvm_ir: generate_llvm_ir.c parser-v1.o lexer-v1.o
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -f generate_llvm_ir *.o

test: compile_test_program
	./test-programs-out/test-program-1

test_program_1: generate_llvm_ir
	./generate_llvm_ir < ./test-programs/test-program-1.txt > ./test-programs-out/test-program-1.ll

compile_test_program: test_program_1
	clang -target arm64-apple-macosx12.0.0 \
		./test-programs-out/test-program-1.ll ./print_integer.c \
		-o ./test-programs-out/test-program-1