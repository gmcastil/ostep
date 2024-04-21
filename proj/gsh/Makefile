CC	= gcc
CFLAGS	= -Wall -Wextra -O0 -std=gnu11 -pedantic -fsanitize=address,undefined -g3

all: gsh

gsh: shell.o prompt.o
	$(CC) $(CFLAGS) shell.o prompt.o -lreadline -o gsh

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f gsh
	rm -f *.o

