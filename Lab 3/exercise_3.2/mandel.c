/*
 * mandel.c
 *
 * A program to draw the Mandelbrot Set on a 256-color xterm.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#include "mandel-lib.h"
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>

#define MANDEL_MAX_ITERATION 100000

/***************************
 * Compile-time parameters *
 ***************************/

/*
 * Output at the terminal is is x_chars wide by y_chars long
*/
int y_chars = 50;
int x_chars = 90;

/*
 * The part of the complex plane to be drawn:
 * upper left corner is (xmin, ymax), lower right corner is (xmax, ymin)
*/
double xmin = -1.8, xmax = 1.0;
double ymin = -1.0, ymax = 1.0;
	
/*
 * Every character in the final output is
 * xstep x ystep units wide on the complex plane.
 */
double xstep;
double ystep;

/*
 * This function computes a line of output
 * as an array of x_char color values.
 */
void compute_mandel_line(int line, int color_val[])
{
	/*
	 * x and y traverse the complex plane.
	 */
	double x, y;

	int n;
	int val;

	/* Find out the y value corresponding to this line */
	y = ymax - ystep * line;

	/* and iterate for all points on this line */
	for (x = xmin, n = 0; n < x_chars; x+= xstep, n++) {

		/* Compute the point's color value */
		val = mandel_iterations_at_point(x, y, MANDEL_MAX_ITERATION);
		if (val > 255)
			val = 255;

		/* And store it in the color_val[] array */
		val = xterm_color(val);
		color_val[n] = val;
	}
}

/*
 * This function outputs an array of x_char color values
 * to a 256-color xterm.
 */
void output_mandel_line(int fd, int color_val[])
{
	int i;
	
	char point ='@';
	char newline='\n';

	for (i = 0; i < x_chars; i++) {
		/* Set the current color, then output the point */
		set_xterm_color(fd, color_val[i]);
		if (write(fd, &point, 1) != 1) {
			perror("compute_and_output_mandel_line: write point");
			exit(1);
		}
	}

	/* Now that the line is done, output a newline character */
	if (write(fd, &newline, 1) != 1) {
		perror("compute_and_output_mandel_line: write newline");
		exit(1);
	}
}

/*-----------------------------------------------------------------*/

/* 
 * POSIX thread functions do not return error numbers in errno,
 * but in the actual return value of the function call instead.
 * This macro helps with error reporting in this case.
 */
#define perror_pthread(ret, msg) \
	do { errno = ret; perror(msg); } while (0)


void usage(char *argv0)
{
	fprintf(stderr, "Usage: %s wrong input!\n\n"
		"Exactly one argument required:\n"
		"    NTHREADS: The number of threads to create.\n",
		argv0);
	exit(1);
}

void *safe_malloc(size_t size)
{
	void *p;

	if ((p = malloc(size)) == NULL) {
		fprintf(stderr, "Out of memory, failed to allocate %zd bytes\n",
			size);
		exit(1);
	}

	return p;
}

struct thread_info_struct {
	pthread_t tid;      /* POSIX thread id, as returned by the library */        
    int thrcnt;         /* number of thread */
};

//declarations 
sem_t *semaphores;                 // semaphores
int NTHREADS;               	  // number of threads


void *compute_and_output_mandel_line(void *arg)
{
	struct thread_info_struct *thr = arg;

	int line = thr->thrcnt;
	int i;
	/*
	 * A temporary array, used to hold color values for the line being drawn
	*/ 
	int color_val[x_chars];
	
	/* thread i calculates lines: i, i + 1*NTHREADS, i + 2*NTHREADS...*/
	/* if line >  y_chars -> thread i has computed all of its lines */
	/* thread i releases semaphore for thread i+1 */
	for(i = line; i < y_chars; i+=NTHREADS) {
		compute_mandel_line(i,color_val);
		sem_wait(&semaphores[i % NTHREADS]);
		output_mandel_line(1, color_val);   //critical section
		sem_post(&semaphores[(i+1) % NTHREADS]);
	}
	return NULL;
}


int safe_atoi(char *s, int *val)
{
	long l;
	char *endp;

	l = strtol(s, &endp, 10);
	if (s != endp && *endp == '\0') {
		*val = l;
		return 0;
	} else
		return -1;
}

/* function to detect signal (ex. Ctrl C) and reset color */
void my_handler(sig_t s) {
	printf("Caught signal %d\n", s);
	reset_xterm_color(1);
	exit(1);
}

int main(int argc, char *argv[]) {

    int i, ret;

    /*
	 * Parse the command line
	 */
	if (argc != 2)
		usage(argv[0]);
	if (safe_atoi(argv[1], &NTHREADS) < 0 || NTHREADS <= 0) {
		fprintf(stderr, "`%s' is not valid for\n", argv[1]);
		exit(1);
	}

	signal (SIGINT,my_handler);

	if(NTHREADS > y_chars) {
		printf("Too many threads! I can accept %i threads!\n", y_chars);
		exit(1);
	}

  	struct thread_info_struct threads[NTHREADS];   // threads

	xstep = (xmax - xmin) / x_chars;
	ystep = (ymax - ymin) / y_chars;


    /* allocate memory for semaphores */
    semaphores = safe_malloc(NTHREADS * sizeof(sem_t));


    /* initialize semaphores */

	// #semaphores = NTHREADS
	
	/* the semaphore for the 1st thread (thread 0) , */
	/* needs to start with value = 1 */ 
	sem_init(&semaphores[0],0,1); 		 

	// the rest of them start with value = 0
    for(i=1; i<NTHREADS; i++) 
		sem_init(&semaphores[i],0,0);

	//create threads
	for(i=0; i < NTHREADS; i++) {

		threads[i].thrcnt = i;
		ret = pthread_create(&threads[i].tid, NULL, compute_and_output_mandel_line, &threads[i]);
		if (ret) {
			perror_pthread(ret, "pthread_create");
			exit(1);
		}
	}

    /*
	 * Wait for all threads to terminate
	 */
	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_join(threads[i].tid, NULL);
		if (ret) {
			perror_pthread(ret, "pthread_join");
			exit(1);
		}
	}

    // destroy semaphores
    for(i=0; i<NTHREADS; i++) 
        sem_destroy(&semaphores[i]);

    reset_xterm_color(1);
	return 0;
}