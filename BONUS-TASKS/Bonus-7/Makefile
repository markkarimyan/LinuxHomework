CC = gcc
CFLAGS = -Wall

all: init destroy set check

init: init.c
	$(CC) $(CFLAGS) -o init init.c

destroy: destroy.c
	$(CC) $(CFLAGS) -o destroy destroy.c

set: set.c
	$(CC) $(CFLAGS) -o set set.c

check: check.c
	$(CC) $(CFLAGS) -o check check.c

clean:
	rm -f init destroy set check
