#
# Makefile
# 

CC = gcc

# CAUTION: Always use '-pthread' when compiling POSIX threads-based
# applications, instead of linking with "-lpthread" directly.
CFLAGS = -Wall -O2 -pthread
LIBS = 

all: mandel-fork mandel-fork2


## Mandel

mandel-fork: mandel-lib.o mandel-fork.o
	$(CC) $(CFLAGS) -o mandel-fork mandel-lib.o mandel-fork.o $(LIBS)

mandel-lib.o: mandel-lib.h mandel-lib.c
	$(CC) $(CFLAGS) -c -o mandel-lib.o mandel-lib.c $(LIBS)

mandel-fork.o: mandel-fork.c
	$(CC) $(CFLAGS) -c -o mandel-fork.o mandel-fork.c $(LIBS)

mandel-fork2: mandel-lib.o mandel-fork2.o
	$(CC) $(CFLAGS) -o mandel-fork2 mandel-lib.o mandel-fork2.o $(LIBS)

mandel-fork.o2: mandel-fork2.c
	$(CC) $(CFLAGS) -c -o mandel-fork2.o mandel-fork2.c $(LIBS)

clean:
	rm -f *.s *.o mandel-fork 
