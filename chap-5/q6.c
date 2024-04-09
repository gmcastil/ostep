#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define errexit(msg) \
	do { \
		fprintf(stderr, "Error: %s\n", msg); \
		exit(EXIT_FAILURE); \
	} while (0)

int main(int argc, char *argv[])
{
	pid_t cpid;

	cpid = fork();
	if (cpid < 0) {
		errexit(strerror(errno));
	} else if (cpid == 0) {
		/* Child process */
		fprintf(stdout, "Child process entered\n");
		exit(EXIT_SUCCESS);
	} else {
		/* Parent process */
		waitpid(cpid, NULL, 0);
		fprintf(stdout, "Child process exited\n");
		exit(EXIT_SUCCESS);
	}
}

