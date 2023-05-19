#
# Makefile
# 

CC = gcc

# CAUTION: Always use '-pthread' when compiling POSIX threads-based
# applications, instead of linking with "-lpthread" directly.
CFLAGS = -Wall -O2 -pthread
LIBS = 

all: mandel mandel_init 

## Mandel & Mandel_initial
mandel_init: mandel-lib.o mandel_init.o
	$(CC) $(CFLAGS) -o mandel_init mandel-lib.o mandel_init.o $(LIBS)

mandel_init.o: mandel_init.c
	$(CC) $(CFLAGS) -c -o mandel_init.o mandel_init.c $(LIBS)

mandel: mandel-lib.o mandel.o
	$(CC) $(CFLAGS) -o mandel mandel-lib.o mandel.o $(LIBS)

mandel-lib.o: mandel-lib.h mandel-lib.c
	$(CC) $(CFLAGS) -c -o mandel-lib.o mandel-lib.c $(LIBS)

mandel.o: mandel.c
	$(CC) $(CFLAGS) -c -o mandel.o mandel.c $(LIBS)

clean:
	rm -f *.s *.o  mandel 
