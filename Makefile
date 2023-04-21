CC=gcc
CFLAGS=-Wall -g

parser-v1: parser-v1.c
	$(CC) $(CFLAGS) -o $@.out $^
