#
# Makefile
# 

CC = gcc

# CAUTION: Always use '-pthread' when compiling POSIX threads-based
# applications, instead of linking with "-lpthread" directly.
CFLAGS = -Wall -O2 -pthread
LIBS = 

all: kgarten kgarten_init

## Kindergarten
kgarten_init: kgarten_init.o
	$(CC) $(CFLAGS) -o kgarten_init kgarten_init.o $(LIBS)

kgarten_init.o: kgarten_init.c
	$(CC) $(CFLAGS) -c -o kgarten_init.o kgarten_init.c

kgarten: kgarten.o
	$(CC) $(CFLAGS) -o kgarten kgarten.o $(LIBS)

kgarten.o: kgarten.c
	$(CC) $(CFLAGS) -c -o kgarten.o kgarten.c

clean:
	rm -f *.s *.o kgarten  
