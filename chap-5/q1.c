#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Write a program that calls fork(). Before calling fork(), have the main
 * process access a variable (e.g., x) and set its value to something (e.g.,
 * 100). What value is the variable in the child process?  What happens to the
 * variable when both the child and parent change the value of x?
 */

int main(int argc, char *argv[])
{
	int x;
	int rc;

	x = 100;
	rc = fork();
	if (rc < 0) {
		fprintf(stderr, "Could not fork process\n");
		exit(1);
	} else if (rc == 0) {
		/* child process */
		printf("Child: value of x is %d\n", x);
		printf("Child changing x\n");
		x++;
		printf("Child: value of x is %d\n", x);
		exit(0);
	} else {
		/* parent process */
		printf("Parent: value of x is %d\n", x);
		printf("Parent changing x\n");
		x++;
		printf("Parent: value of x is %d\n", x);
		exit(0);
	}

	return 0;
}
