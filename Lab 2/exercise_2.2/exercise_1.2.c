#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/wait.h>

#include "tree.h"
#include "proc-common.h"

#define SLEEP_PROC_SEC  10
#define SLEEP_TREE_SEC  3

void fork_tree(struct tree_node *root) {

    /* exit(17) -> leaves
       exit(42) -> parents */

    change_pname(root->name);
    int i, status;

    /* case 1 -> Current node is a leaf */
    if(root->nr_children == 0) {
        printf("Leaf %s -> Created\n", root->name);
        printf("Leaf %s -> Sleeping\n", root->name);
        sleep(SLEEP_PROC_SEC);
        printf("Leaf %s -> Terminating\n", root->name);
        exit(17);  
    }
    else {
        /* case 2 -> Current node has child(ren) */

        printf("Node %s -> Created\n", root->name);
        pid_t child;

        for(i = 0; i < root->nr_children; i++) {
            child = fork();
            if(child < 0) {
                /* child fork failed */
                perror("child fork failed");
                exit(1);
            }
            else if(child == 0) {
                /* In child process */
                fork_tree(root->children + i);
            }
            /* else continue to next iteration or exit loop */
        }
        
        /* In father process */ 

        printf("Node %s -> Waiting\n", root->name);
        for(i = 0; i < root->nr_children; i++) {
            child = wait(&status);
            explain_wait_status(child, status);
        }
        printf("Node %s -> Terminating\n", root->name);
        exit(42); 
    }  
}


int main(int argc, char *argv[])
{
        struct tree_node *root;

        /* Checking for valid input */
        if (argc != 2) {
                fprintf(stderr, "Usage: %s <input_tree_file>\n\n", argv[0]);
                exit(1);
        }

        root = get_tree_from_file(argv[1]);
        print_tree(root);

        pid_t pid;
        int status;

        pid = fork();
        if(pid < 0) {
            perror("main: fork");
            exit(1);
        }
        if(pid == 0) {
            /* Child */
            fork_tree(root);
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