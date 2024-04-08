CC	= gcc
CFLAGS	= -Wall -Wextra -O0 -g -std=gnu11 -pedantic -Wno-implicit-fallthrough -Wno-unused-parameter

SRC = $(wildcard *.c)
EXE = $(SRC:.c=)

all: $(EXE)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(EXE)

