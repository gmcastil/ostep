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
	pid_t rc;

        cpid = fork();
        if (cpid < 0) {
                errexit(strerror(errno));
        } else if (cpid == 0) {
                /* Child process */
		if (close(STDOUT_FILENO) == -1) {
			errexit(strerror(errno));
		}
		fprintf(stdout, "Child: Should not be printed\n");
		exit(EXIT_SUCCESS);
        } else {
                /* Parent process */
		rc = wait(&cpid);
		fprintf(stdout, "Parent: Child with PID %d is done\n", rc);
        }

}


