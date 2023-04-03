#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tree.h"
#include "proc-common.h"

void fork_DFS(struct tree_node *root)
{
        /*
         * Start
         */

        int i, status;

        printf("PID = %ld, name %s, starting...\n",
                        (long)getpid(), root->name);
        change_pname(root->name);

        /* case 1 -> Current node is a leaf */
        if(root->nr_children == 0) {

            /* Suspend Self */   
            raise(SIGSTOP); /* stop */
            
            /* after awakening */
            printf("PID = %ld, name = %s is awake\n",
                    (long)getpid(), root->name);
            exit(0);
        }
        else {
            /* case 2 -> Current node has child(ren) */

            /* kids -> array with PIDs of the children of current node */
            pid_t kids[root->nr_children];  

            for(i = 0; i < root-> nr_children; i++) {
                kids[i] = fork();
                if(kids[i] < 0) {
                    /* child fork failed */
                    perror("child fork failed");
                    exit(1);
                }
                else if(kids[i] == 0) {
                    /* In child process */
                    fork_DFS(root->children + i);
                }
            }
            /* In father process */

            /* wait for ALL children to stop */
            wait_for_ready_children(root->nr_children);

            raise(SIGSTOP); /* stop */

            /* after awakening */
            printf("PID = %ld, name = %s is awake\n",
                (long)getpid(), root->name);

            for(i = 0; i < root->nr_children; i++) {
                /* wake up children */
                kill(kids[i], SIGCONT);
                printf("PID = %ld, name = %s is waiting\n",
                    (long)getpid(), root->name);
                kids[i] = wait(&status);
                explain_wait_status(kids[i], status);
            }
            exit(0);
        }
}


/*
 * The initial process forks the root of the process tree,
 * waits for the process tree to be completely created,
 * then takes a photo of it using show_pstree().
 *
 * In ask2-signals:
 *      use wait_for_ready_children() to wait until
 *      the first process raises SIGSTOP.
 */

int main(int argc, char *argv[])
{
        pid_t pid;
        int status;
        struct tree_node *root;

        if (argc < 2){
                fprintf(stderr, "Usage: %s <tree_file>\n", argv[0]);
                exit(1);
        }

        /* Read tree into memory */
        root = get_tree_from_file(argv[1]);
        print_tree(root);

        /* Fork root of process tree */
        pid = fork();
        if (pid < 0) {
                perror("main: fork");
                exit(1);
        }
        if (pid == 0) {
                /* Child */
                fork_DFS(root);
                exit(1);
        }

        /*
         * Father
         */

        /* for ask2-signals */
        wait_for_ready_children(1);

        /* Print the process tree root at pid */
        show_pstree(pid);

        /* for ask2-signals */
        kill(pid, SIGCONT);   /* send SIGCONT signal to root process */

        /* Wait for the root of the process tree to terminate */
        pid = wait(&status);
        explain_wait_status(pid, status);

        return 0;
}