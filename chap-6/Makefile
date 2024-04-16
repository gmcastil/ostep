CC	= gcc
CFLAGS	= -Wall -Wextra -O0 -g -std=gnu11 -pedantic

SRC = $(wildcard *.c)
EXE = $(SRC:.c=)

all: $(EXE)

# Need finer tuning on compilation of this one
q2.s: q2.c
	$(CC) $(USER_DEFINES) -Wall -Wextra -O0 -S -g -std=gnu11 -pedantic -m64 -I../include $< -o $@

q2: q2.c
	$(CC) $(USER_DEFINES) -Wall -Wextra -O0 -g -std=gnu11 -pedantic -m64 -I../include $< -o $@

%: %.c
	$(CC) $(USER_DEFINES) $(CFLAGS) -I../include $< -o $@

.PHONY: clean
clean:
	rm -f $(EXE)

