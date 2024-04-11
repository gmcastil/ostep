CC	= gcc
CFLAGS	= -Wall -Wextra -O0 -g -std=gnu11 -pedantic -Wno-unused-parameter

SRC = $(wildcard *.c)
EXE = $(SRC:.c=)

all: $(EXE)

q2: q2.c
	$(CC) $(CFLAGS) -arch x86_64 -I../include $< -o $@

%: %.c
	$(CC) $(CFLAGS) -I../include $< -o $@

.PHONY: clean
clean:
	rm -f $(EXE)

