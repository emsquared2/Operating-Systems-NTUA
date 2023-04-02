#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

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
		doWrite(fd, buff, rcnt);
	}

	close(o_fd);
}