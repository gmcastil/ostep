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
}

static void greet_with_signal()
{
	/* We choose how to handle a signal (which signal?) using a handler
	 * function ... signal or sigaction()
	 *
	 * so...what kind of signal does the child need to send to the parent?
	 * one of the miscellaneous signals (SIGUSR1)
	 * how does the parent wait until receiving that signal?
	 * the parent needs to call the signal() function and establish the
	 * signal handler as the action for the signal from teh child.
	 * Nothing wrong with putting my reply in the signal handler.kkkkkkkk
	 *
	 * We need to wait (block?) until receiving the right siganal, so
	 * sigaction() is the deal i think
	 */

	struct sigaction usr_action;
	sigset_t block_mask;
	sigset_t mask;

	pid_t cpid;

	/* See https://www.gnu.org/software/libc/manual/html_node/Kill-Example.html */

	/* Establish the signal handler */
	usr_action.sa_handler = usr_sig_action;
	

	cpid = fork();
	if (cpid < 0) {
		/* Call to fork() failed */
		errexit(strerror(errno));
	} else if (cpid == 0) {
		/* Child process */
		printf("Hello\n");
		kill(getppid(), SIGUSR1);
	} else {
		/* Parent process */

		/* Block only the USR1 signals */
		sigemptyset(&block_mask);
		if (sigaddset(&block_mask, SIGUSR1) == -1) {
			errexit(strerror(errno));
		}

		while (!usr_sig_flag) {
			/* 
			 * Note that sigsuspend() restores the previous signal
			 * mask when it returns.
			 */
			sigsuspend(&
		}
		printf("Received signal %d. Goodbye\n", signum);
	}
		

	/*
	 * If we're the parent, then wait until receiving a signal, then
	 * greet, and then exit
	 */
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
