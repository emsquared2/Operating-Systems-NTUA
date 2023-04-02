#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

int write_file(int fd, const char *infile);

int main(int argc, char **argv) {
	if(argc == 2 || argc == 1) 
		printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)] \n");
	else {
		int fd_out, oflags, mode;
		oflags = O_CREAT | O_WRONLY | O_TRUNC;
		mode = S_IRUSR | S_IWUSR;
		fd_out = open(argv[argc-1], oflags, mode);
		if(fd_out == -1) {
	         	perror("open");
				exit(1);
			}
		int i;
		for(i = 1; i< argc-1; i++) {
			write_file(fd_out, argv[i]);
			       }
		close(fd_out);
		exit(0);
	
			
        	}  
	return 0;
}

