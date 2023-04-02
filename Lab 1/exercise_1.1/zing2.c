#include <unistd.h>
#include <stdio.h>

void zing(void) {
	printf("Hello Manos and Giannis, your username is %s!\n", getlogin());
}

