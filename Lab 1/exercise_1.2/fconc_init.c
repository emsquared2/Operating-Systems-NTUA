#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int doWrite(int fd, const char *buff, int len) {
	size_t  idx = 0;
	ssize_t wcnt;
	do {
		wcnt = write(fd, buff + idx, len - idx);
		if(wcnt == -1) { /* error */
		   perror("write");
   		   return 1;
		}
		idx += wcnt;
	} while (idx < len);
	return 0;
}	


int write_file(int fd, const char *infile) {
	int o_fd;
	o_fd = open(infile, O_RDONLY);
	if (o_fd == -1) {
		perror("open");
		exit(1);
	}
	char buff[1024];
	ssize_t rcnt;
	for (;;) { 
		rcnt = read(o_fd, buff, sizeof(buff)-1);
		if(rcnt == 0) /* end-of-file */
			return 0;
		if(rcnt == -1) { /* error */ 
			perror("read");
			return 1;
		}
		buff[rcnt] = '\0';
		int len = strlen(buff);
		doWrite(fd, buff, len);
	}
	close(o_fd);
}

	 
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