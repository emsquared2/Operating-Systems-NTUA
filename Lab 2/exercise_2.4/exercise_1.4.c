#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>

#include <sys/prctl.h>
#include <sys/wait.h>

#include "proc-common.h"
#include "tree.h"


int compute_value(int x, int y, char operator) {
    if(operator == '*') 
        return x * y;
    else 
        return x + y;
}


void fork_pipe(struct tree_node *root, int pfd) {

    if(root->nr_children > 2) {
        perror("More than two children -> Wrong input!");
        exit(1);
    }

    int i, status;

    printf("PID = %ld, name %s, starting...\n",
                    (long)getpid(), root->name);
    change_pname(root->name);

    /* case 1 -> current node is a leaf (integer) */
    if(root->nr_children == 0) {

        printf("Value = %s\n", root->name);
        int value = atoi(root->name);  /* convert to integer */

        /* leaf performs write */
        if ( write(pfd, &value, sizeof(value)) != sizeof(value) ) {
            printf("%s -> Write failed\n", root->name);
            exit(1);
        }
        printf("Leaf with PID: %ld ,Name: %s wrote %i in pipe\n", (long)getpid(),root->name, value);
        printf("Leaf with PID: %ld ,Name: %s -> Terminating\n", (long)getpid(), root->name);
        exit(0);
    }
    else {
        /* case 2 -> current node has child(ren) */

        /* kids -> array with PIDs of the children of current node */
        pid_t kids[root->nr_children]; 

        /* Parent Creating pipe */
        int kid_pfd[2];

	    printf("Parent %ld ,Name: %s: Creating pipe...\n", (long)getpid(), root->name);
	    if (pipe(kid_pfd) < 0) {
		    perror("pipe");
		    exit(1);
        }
        for(i = 0; i < root->nr_children; i++) {
            kids[i] = fork();
            if(kids[i] < 0) {
                /* child fork failed */
                perror("child fork failed");
                exit(1);
            }
            else if (kids[i] == 0) {
                /* In child process */
                close(kid_pfd[0]);  /*  child closes reading edge */
                fork_pipe(root->children + i, kid_pfd[1]); /* child writes */
            }
        }
        /* In parent process */

        close(kid_pfd[1]);  /* parent closes writing edge */ 
        /* children's values */
        int values[2];

        for(i = 0; i < root->nr_children; i++) {

            if( read(kid_pfd[0], &values[i], sizeof(values[i])) != sizeof(values[i]) ){
                printf("%s -> Read failed\n", root->name);
                exit(1);
            }
            printf("PID = %ld, name = %s is waiting\n", (long)getpid(), root->name);
            kids[i] = wait(&status);
            explain_wait_status(kids[i], status);
        }
        int new_value = compute_value(values[0], values[1], *(root->name));
        printf("PID = %ld, new value = %i\n", (long)getpid(), new_value);

        /* perform write */
        if ( write(pfd, &new_value, sizeof(new_value)) != sizeof(new_value) ) {
            printf("%s -> Write failed\n", root->name);
            exit(1);
        }
        printf("Node with PID: %ld , Name: %s ,wrote %i in pipe\n", (long)getpid(), root->name, new_value);
        printf("Node with PID: %ld, Name %s -> Terminating\n", (long)getpid(), root->name);
        exit(0);
    }
}



int main(int argc, char *argv[])
{
	pid_t pid;
	int pfd[2];
	int status;
    struct tree_node *root;

    /* Checking for valid input */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_tree_file>\n\n", argv[0]);
        exit(1);
    }

    /* Read tree into memory */
    root = get_tree_from_file(argv[1]);
    print_tree(root);

    /* Parent Creating pipe */

	printf("Parent %ld: Creating pipe...\n", (long)getpid());
	if (pipe(pfd) < 0) {
		perror("pipe");
		exit(1);
	}

	printf("Parent %ld: Creating child...\n", (long)getpid());
	pid = fork();
	if (pid < 0) {
		perror("main:fork");
		exit(1);
	}
	if (pid == 0) {
		/* In child process */
		fork_pipe(root, pfd[1]);  /* child writes */
		/*
		 * Should never reach this point,
		 * fork_pipe() does not return
		 */
        printf("Child should never reach this point\n");
		assert(0);
	}

	/*
	 * In parent process.
	 */
	
    int final_value;
    if( read(pfd[0], &final_value, sizeof(final_value)) != sizeof(final_value) ){
        printf("Read failed\n");
        exit(1);
    }


    close(pfd[1]);  /* parent closes writing edge */ 


	/* Wait for the root of the process to terminate */
	pid = wait(&status);
	explain_wait_status(pid, status);
	
    printf("Final Value = %i\n", final_value);
	printf("All done, exiting...\n");

	return 0;
}