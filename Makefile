all: aring.o

CFLAGS=-Os -std=c11

asm: aring.s

aring.s:
	cc -S -std=c11 -Os aring.c

clean:
	rm aring.[os]
