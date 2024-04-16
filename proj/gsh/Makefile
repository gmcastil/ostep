CC	= gcc
CFLAGS	= -Wall -Wextra -O0 -g -std=gnu11 -pedantic

all: gsh

gsh: shell.o parse.o
	$(CC) $(CFLAGS) shell.o parse.o -o gsh

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f gsh
	rm -f *.o

