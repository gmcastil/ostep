#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#define errexit(msg) \
	do { \
		fprintf(stderr, "Error: %s\n", msg); \
		exit(EXIT_FAILURE); \
	} while (0)

static const char *cmd = "/bin/ls";

int main(int argc, char *argv[])
{
	int sel;
	int rc_wait;

	pid_t cpid;

	printf("Choose exec variant:\n");
	printf("\n");
	printf("  1) execl()\n");
	printf("  2) execle()\n");
	printf("  3) execlp()\n");
	printf("  4) execv()\n");
	printf("  5) execvp()\n");
	printf("  6) execvpe()\n");
	printf("\n");
	printf("Enter selection: ");

	scanf("%d", &sel);

	cpid = fork();
	if (cpid < 0) {
		/* Call to fork() failed */
		errexit(strerror(errno));
	} else if (cpid == 0) {
		/* Child process */

		switch (sel) {

			/* execl() */
			case 1:
				execl(cmd, "-al", NULL);

			/* execle() */
			case 2:

			/* execlp() */
			case 3:

			case 4:

			case 5:

			case 6:

			default:
		}
			

	} else {
		/* Parent process */
		rc_wait = wait(NULL);
	}

	return 0;

}
