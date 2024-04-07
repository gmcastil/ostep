#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define errexit(msg) \
	do { \
		fprintf(stderr, "Error: %s\n", msg); \
		exit(EXIT_FAILURE); \
	} while (0)

/* Interprocess communiation flag */
static sig_atomic_t usr_sig_flag = 0;

_Noreturn static void usage(char *name)
{
	fprintf(stderr, "Usage: %s [-s|--signal] [-p|--pipe] \n", name);
	exit(EXIT_FAILURE);
}

static void usr_sig_handler(int signum)
{
	usr_sig_flag = 1;
}

static void greet_with_pipe()
{
	pid_t cpid;
	int fds[2];

	char msg[4];

	/* 
	 * Key detail here is that child and parent processes all share the same
	 * file descriptors and that the default behavior of a pipe is to block
	 * when reading (i.e., blocks until the read from the pipe is actually
	 * performed).
	 */
	if (pipe(fds) == -1) {
		errexit(strerror(errno));
	}

	cpid = fork();
	if (cpid < 0) {
		/* Call to fork() failed */
		errexit(strerror(errno));
	} else if (cpid == 0) {
		/* Child process */
		fprintf(stdout, "Hello. ");
//		close(fds[0]);
		write(fds[1], "Done", sizeof(msg));
		close(fds[1]);
	} else {
		/* Parent process */
//		close(fds[1]);
		read(fds[0], &msg, sizeof(msg));
		close(fds[0]);
		fprintf(stdout, "Pipe message \'%s\' received. Goodbye\n", msg);
	}
}

static void greet_with_signal()
{

	struct sigaction usr_action;
	sigset_t block_mask;
	sigset_t old_mask;

	pid_t cpid;

	/* See https://www.gnu.org/software/libc/manual/html_node/Kill-Example.html */

	/*
	 * Per man sigemptyset (3), objects of type sigset_t must be initialized
	 * prior to being passed to functions like sigaddset() or the results
	 * are undefined
	 * 
	 * First block the SIGUSR1 signal before we set up the handler
	 */
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGUSR1);
	/* 
	 * Key detail here is that the signal mask prior to blocking is returned
	 * in old_mask (see man sigprocmask for more details)
	 */
	if (sigprocmask(SIG_BLOCK, &block_mask, &old_mask) == -1) {
		errexit(strerror(errno));
	}

	/* Now, configure the signal handler */
	usr_action.sa_handler = usr_sig_handler;
	/* 
	 * Per man sigaction (2), the signal which triggered the handler is
	 * blocked in the handler unless the SA_NODEFER flag is used, so that
	 * can be empty.
	 */
	sigemptyset(&usr_action.sa_mask);
	usr_action.sa_flags = 0;
	if (sigaction(SIGUSR1, &usr_action, NULL) == -1 ) {
		errexit(strerror(errno));
	}

	cpid = fork();
	if (cpid < 0) {
		/* Call to fork() failed */
		errexit(strerror(errno));
	} else if (cpid == 0) {
		/* Child process */
		printf("Hello. ");
		kill(getppid(), SIGUSR1);
	} else {
		/* Parent process */
		while (!usr_sig_flag) {
			/* Now suspend until delivery of the desired signal */
			sigsuspend(&old_mask);
		}
		printf("Received signal. Goodbye\n");
	}
}

int main(int argc, char *argv[])
{
	int c;

	struct option options[] = {
		{"signal", no_argument, NULL, 's'},
		{"pipe", no_argument, NULL, 'p'}
	};

	
	if ((c = getopt_long(argc, argv, "ps", options, NULL)) == -1) {
		usage(argv[0]);
		return 1;	
	} else {
		switch (c) {
			case 'p':
				greet_with_pipe();
				return 0;
			case 's':
				greet_with_signal();
				return 0;
			case '?':
				printf("unknown arg\n");
				usage(argv[0]);
			default:
				usage(argv[0]);
		}
	}

	return 0;
}
