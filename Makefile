CC=gcc
CFLAGS=-Wall -g

all: generate_llvm_ir

generate_llvm_ir: generate_llvm_ir.c parser-v1.o
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -f generate_llvm_ir *.o

test: generate_llvm_ir
	./generate_llvm_ir < ./test-programs/test-program-1.txt