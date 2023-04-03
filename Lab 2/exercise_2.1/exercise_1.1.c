#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "proc-common.h"

#define SLEEP_PROC_SEC  10
#define SLEEP_TREE_SEC  3

/*
 * Create this process tree:
 * A-+-B---D
 *   `-C
 */

void fork_procs(void)
{
        /* initial process is A. */

        pid_t B, C, D;
        int status;

        change_pname("A");
        printf("A -> Created\n");

        /* we create B */
        B = fork();
        if(B < 0) {
            /* B fork failed */
            perror("B fork failed");
            exit(1);
        }
        else if(B == 0) {
            /* In B process */
            change_pname("B");
            printf("B -> Created\n");

            /* we create D */
            D = fork();
            if(D < 0) {
                /* D fork failed */
                perror("D fork failed");
                exit(1);
            }
             else if(D == 0) {
                /* In D process */
                change_pname("D");
                printf("D -> Created\n");
                printf("D -> Sleeping\n");
                sleep(SLEEP_PROC_SEC);
                printf("D -> Terminating\n");
                exit(13);
            }
            else {
                /* In B process */
                printf("B -> Waiting\n");
                D = wait(&status);
                explain_wait_status(D, status);
                printf("B -> Terminating\n");
                exit(19);
            }
        }
        else {
            /* In A process */

            /* we create C */
            C = fork();
            if(C < 0) {
                /* C fork failed */
                perror("C fork failed");
                exit(1);
            }
            else if(C == 0) {
                /* In C process */
                change_pname("C");
                printf("C -> Created\n");
                printf("C -> Sleeping\n");
                sleep(SLEEP_PROC_SEC);
                printf("C -> Terminating\n");
                exit(17);
            }
            else {
                /* In A process */

                printf("A -> Waiting\n");
                B = wait(&status);
                explain_wait_status(B, status);
                C = wait(&status);
                explain_wait_status(C, status);
                 printf("A -> Terminating\n");
                exit(16);
            }
        }
}

/*
 * The initial process forks the root of the process tree,
 * waits for the process tree to be completely created,
 * then takes a photo of it using show_pstree().
 *
 * How to wait for the process tree to be ready?
 * In ask2-{fork, tree}:
 *      wait for a few seconds, hope for the best.
 */

int main(void)
{
        pid_t pid;
        int status;

        /* Fork root of process tree */
        pid = fork();
        if (pid < 0) {
                perror("main: fork");
                exit(1);
        }
        if (pid == 0) {
                /* Child */
                fork_procs();
                exit(1);
        }

        /* Father */

        /* for ask2-{fork, tree} */
        sleep(SLEEP_TREE_SEC);

        /* Print the process tree root at pid */
        show_pstree(pid);

        /* Wait for the root of the process tree to terminate */
        pid = wait(&status);
        explain_wait_status(pid, status);

        return 0;
}