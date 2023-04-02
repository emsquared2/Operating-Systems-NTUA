all: fconc fconc2

fconc: fconc_help.o fconc.o
	gcc -o fconc fconc_help.o fconc.o

fconc_help.o: fconc_help.c
	gcc -Wall -c fconc_help.c

fconc.o: fconc.c
	gcc -Wall -c fconc.c

fconc2: fconc_help.o fconc2.o
	gcc -o fconc2 fconc_help.o fconc2.o

fconc2.o: fconc2.c
	gcc -Wall -c fconc2.c

clean:
	rm fconc fconc.o fconc_help.o fconc2 fconc2.o