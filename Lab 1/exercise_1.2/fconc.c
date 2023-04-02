#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

int write_file(int fd, const char *infile);

int main(int argc, char **argv) {

	if(argc != 3 && argc != 4) 
		printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)] \n");
	else {

		int fd_out, oflags, mode;
		oflags = O_CREAT | O_WRONLY | O_TRUNC;
		mode = S_IRUSR | S_IWUSR;

		if(argc == 3) {
			fd_out = open("fconc.out", oflags, mode);
			if(fd_out == -1) {
				perror("open");
				exit(1);
			}
			write_file(fd_out, argv[1]);
			write_file(fd_out, argv[2]);
			close(fd_out);
			exit(0);
		}
		else {
			
			fd_out = open(argv[3], oflags, mode);
			if(fd_out == -1) {
				perror("open");
				exit(1);
			}
			write_file(fd_out, argv[1]);
			write_file(fd_out, argv[2]);
			close(fd_out);
			exit(0);
		}	
	}
	return 0;
}

