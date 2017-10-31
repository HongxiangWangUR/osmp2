# Commands
CC := gcc
RM := rm -rf

# CFLAGS
CFLAGS := -static -O2

# Targets
all: test expr1 expr2 expr3

test: test.c prinfo.c prinfo.h
	$(CC) $(CFLAGS) -o test prinfo.c test.c

expr1: expr1.c
	$(CC) $(CFLAGS) -o expr1 expr1.c

expr2: expr2.c prinfo.c prinfo.h
	$(CC) $(CFLAGS) -o expr2 prinfo.c expr2.c

expr3: expr3.c prinfo.c prinfo.h
	$(CC) $(CFLAGS) -o expr3 prinfo.c expr3.c

clean:
	$(RM) test expr1 expr2 expr3