#define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "defines.h"

/* Number of signals to pass between parent and child processes */
#define NUM_DEFAULT_ITERATIONS	10

/* This could be passed in as an argument, but I'm running in a VM with 2 cores */
#define NUM_CPUS	2
/* We wish to constrain our process and its children to the last CPU */
#define BOUND_CPU	(NUM_CPUS - 1)

#define PIPE_READ	0
#define PIPE_WRITE	1	

int main(int argc, char *argv[])
{
	int num_iterations;

	if (argc == 1) {
		num_iterations = NUM_DEFAULT_ITERATIONS;
	} else {
		num_iterations = atoi(argv[1]);
	}

	pid_t cpid;

	int pipe_mst[2];
	int pipe_slv[2];

	char msg;
	char buf;

	cpu_set_t mask;

	CPU_ZERO(&mask);
	CPU_SET(BOUND_CPU, &mask);

	if (!CPU_ISSET(BOUND_CPU, &mask)) {
		errexit("Could not set CPU %d in mask", BOUND_CPU);
	}

	if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
		if (errno == EINVAL) {
			errexit("Bit mask contains no processors currently on the system");
		} else {
			errexit(strerror(errno));
		}
	} else {
		printf("Processor afinity bound to CPU ID %d\n", BOUND_CPU);
	}

	/* 
	 * The lmbench benchmark does so by running two processes on a single
	 * CPU, and set- ting up two UNIX pipes between them; a pipe is just one
	 * of many ways processes in a UNIX system can communicate with one
	 * another. The first process then issues a write to the first pipe, and
	 * waits for a read on the second; upon seeing the first process waiting
	 * for something to read from the second pipe, the OS puts the first
	 * process in the blocked state, and switches to the other process,
	 * which reads from the first pipe and then writes to the second. When
	 * the second process tries to read from the first pipe again, it
	 * blocks, and thus the back-and-forth cycle of communication continues.
	 * By measuring the cost of communicating like this repeatedly, lmbench
	 * can make a good estimate of the cost of a context switch.
	*/

	/* Create two pipes */
	if (pipe(pipe_mst) == -1) {
		errexit(strerror(errno));
	}
	if (pipe(pipe_slv) == -1) {
		errexit(strerror(errno));
	}

	cpid = fork();
	if (cpid == -1) {
		errexit(strerror(errno));
	} else if (cpid == 0) {
		/* Child process - The slave */
		msg = 'Y';

		close(pipe_mst[PIPE_WRITE]);
		close(pipe_slv[PIPE_READ]);
		/* Block here until the kernel delivers byte from the master */
		for (int i=0; i<num_iterations; i++) {
			if ( read(pipe_mst[PIPE_READ], &buf, 1) == -1 ) {
				errexit(strerror(errno));
			}
#ifdef PIPE_DEBUG
			else {
				printf("Slave read %c\n", buf);
			}
#endif  // PIPE_DEBUG
			if ( write(pipe_slv[PIPE_WRITE], &msg, 1) == -1 ) {
				errexit(strerror(errno));
			}
#ifdef PIPE_DEBUG
			else {
				printf("Slave wrote %c\n", msg);
			}
#endif  // PIPE_DEBUG
		}

		close(pipe_mst[PIPE_READ]);
		close(pipe_slv[PIPE_WRITE]);
		exit(EXIT_SUCCESS);
	} else {
		/* Parent process - The master */
		msg = 'X';

		struct timeval start_time;
		struct timeval stop_time;
		struct timeval diff_time;

		close(pipe_mst[PIPE_READ]);
		close(pipe_slv[PIPE_WRITE]);

		gettimeofday(&start_time, NULL);
		for (int i=0; i<num_iterations; i++) {
			if ( write(pipe_mst[PIPE_WRITE], &msg, 1) == -1 ) {
			     errexit(strerror(errno));
			}
#ifdef PIPE_DEBUG
			else {
				printf("Master wrote %c\n", msg);
			}
#endif  // PIPE_DEBUG
			/* Block here until the kernel delivers byte from the slave */
			if ( read(pipe_slv[PIPE_READ], &buf, 1) == -1 ) {
				errexit(strerror(errno));
			}
#ifdef PIPE_DEBUG
			else {
				printf("Master read %c\n", buf);
			}
#endif  // PIPE_DEBUG
		}
		gettimeofday(&stop_time, NULL);
		timersub(&stop_time, &start_time, &diff_time);
		/* Each iteration yields two context switches */
		wait(NULL);
		printf("Estimated time for context switch using gettimeofday() is %f us\n",
				(float) diff_time.tv_usec / (num_iterations / 2));

		close(pipe_mst[PIPE_WRITE]);
		close(pipe_slv[PIPE_READ]);
		exit(EXIT_SUCCESS);
	}

	return 0;
}
