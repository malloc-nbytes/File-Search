CC=gcc
CFLAGS=-Werror -Wall -Wextra -std=c99 -pedantic
DEPS=stack.h hash_table.h
OBJ=main.o stack.o hash_table.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CLFAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

