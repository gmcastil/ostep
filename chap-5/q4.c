/* Required for execvpe() */
#define _GNU_SOURCE

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

static char const *env[] = {
	"BLOCKSIZE=512",
	NULL
};

static char *const example_argv[] = {
	"/usr/bin/ls",
	"-s",
	"-l",
	"--color=always",
	NULL};

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
		/* 
		 * Note that the arguments passed here can be thought of as
		 * arg0, arg1, ..., argn. This allows one to provide a differeng
		 * argv[0] than the name of the program (e.g., bash being run as
		 * sh instead). Note that this allows programs to lie to
		 * themselves as to how they were invoked.
		 */

			/* execl() */
			case 1:
				/* We inherit BLOCKSIZE from the parent */
				execl(cmd, cmd, "-sl", "--color=always", (char *) NULL);

			/* execle() */
			case 2:
				/* 
				 * Note that we can specify an environment that
				 * defines BLOCKSIZE=512
				 */
				execle(cmd, cmd, "-sl", (char *) NULL, env);

			/* execlp() */
			case 3:
				/* Let the shell do the searching for us */
				execlp("ls", "/nowhere/ls", "-sl", (char *) NULL);
				/* 
				 * Look at this!  I can lie and claim I was
				 * invoked from one place when I was actually
				 * invoked from a different one!
				 */

			/* execv() */
			case 4:
				execv(cmd, example_argv);

			/* execvp() */
			case 5:
				/*
				 * Same as execv() but we let the shell search
				 * for us
				 */
				execvp("ls", example_argv);

			/* execvpe() */
			case 6:
				execvpe("ls", example_argv, env);

			default:
				{}
		}
			

	} else {
		/* Parent process */
		rc_wait = wait(NULL);
		exit(EXIT_SUCCESS);
	}

	return 0;

}
