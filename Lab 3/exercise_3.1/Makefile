#
# Makefile
# 

CC = gcc

# CAUTION: Always use '-pthread' when compiling POSIX threads-based
# applications, instead of linking with "-lpthread" directly.
CFLAGS = -Wall -O2 -pthread
LIBS = 

all: simplesync-mutex simplesync-atomic simplesync-mutex_init simplesync-atomic_init 


## Simple sync (two versions)
simplesync-mutex: simplesync-mutex.o
	$(CC) $(CFLAGS) -o simplesync-mutex simplesync-mutex.o $(LIBS)

simplesync-atomic: simplesync-atomic.o
	$(CC) $(CFLAGS) -o simplesync-atomic simplesync-atomic.o $(LIBS)

simplesync-mutex.o: simplesync.c
	$(CC) $(CFLAGS) -DSYNC_MUTEX -c -o simplesync-mutex.o simplesync.c

simplesync-atomic.o: simplesync.c
	$(CC) $(CFLAGS) -DSYNC_ATOMIC -c -o simplesync-atomic.o simplesync.c

## Simple sync initial (two versions)
simplesync-mutex_init: simplesync-mutex_init.o
	$(CC) $(CFLAGS) -o simplesync-mutex_init simplesync-mutex_init.o $(LIBS)

simplesync-atomic_init: simplesync-atomic_init.o
	$(CC) $(CFLAGS) -o simplesync-atomic_init simplesync-atomic_init.o $(LIBS)

simplesync-mutex_init.o: simplesync_init.c
	$(CC) $(CFLAGS) -DSYNC_MUTEX -c -o simplesync-mutex_init.o simplesync_init.c

simplesync-atomic_init.o: simplesync_init.c
	$(CC) $(CFLAGS) -DSYNC_ATOMIC -c -o simplesync-atomic_init.o simplesync_init.c

clean:
	rm -f *.s *.o simplesync-{atomic,mutex}