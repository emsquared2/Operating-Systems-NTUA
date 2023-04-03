.PHONY: all clean

all: fork-example tree-example ask2-fork ask2-signals exercise_1.1 exercise_1.2 exercise_1.3 exercise_1.4 exercise_1.42

CC = gcc 
CFLAGS = -g -Wall -O2 
SHELL= /bin/bash

tree-example: tree-example.o tree.o
	$(CC) $(CFLAGS) $^ -o $@ 

fork-example: fork-example.o proc-common.o
	$(CC) $(CFLAGS) $^ -o $@ 

ask2-fork: ask2-fork.o proc-common.o
	$(CC) $(CFLAGS) $^ -o $@ 

ask2-signals: ask2-signals.o proc-common.o tree.o
	$(CC) $(CFLAGS) $^ -o $@ 

exercise_1.1: exercise_1.1.o proc-common.o
	$(CC) $(CFLAGS) $^ -o $@

exercise_1.2: exercise_1.2.o proc-common.o tree.o
	$(CC) $(CFLAGS) $^ -o $@

exercise_1.3: exercise_1.3.o proc-common.o tree.o
	$(CC) $(CFLAGS) $^ -o $@

exercise_1.4: exercise_1.4.o proc-common.o tree.o
	$(CC) $(CFLAGS) $^ -o $@

exercise_1.42: exercise_1.42.o proc-common.o tree.o
	$(CC) $(CFLAGS) $^ -o $@

%.s: %.c
	$(CC) $(CFLAGS) -S -fverbose-asm $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< 

%.i: %.c
	gcc -Wall -E $< | indent -kr > $@

clean: 
	rm -f *.o tree-example fork-example pstree-this ask2-{fork,tree,signals,pipes}
