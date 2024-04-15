#define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "defines.h"

#if defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
/* GCC-compatible compiler, targeting x86/x86-64 */
#include <x86intrin.h>
#endif

/* Sample text file to open for reading 0 bytes */
#define SAMPLE_FILE	"sample.txt"

/* Reference clock frequency */
#define CPU_MHZ		3.609604E9

/* This could be passed in as an argument, but I'm running in a VM with 2 cores */
#define NUM_CPUS	2
/* We wish to constrain our process and its children to the last CPU */
#define BOUND_CPU	(NUM_CPUS - 1)
/* How many samples to measure */
#define NUM_DEFAULT_ITERATIONS		10
/* How many initial values ot skip when measuring */
#define NUM_WARMUP	2

struct meas_results {
	unsigned long long	mean;
	unsigned long long	max;
	unsigned long long	min;
	unsigned int		tsc_aux;
	int			samples;
};

void print_results(struct meas_results *results)
{
	if (results == NULL) {
		errexit("Null pointer check failed");
	} else {
		printf("%-15s%lld\n", "Mean:", results->mean);
		printf("%-15s%lld\n", "Max:", results->max);
		printf("%-15s%lld\n", "Min:", results->min);
		printf("%-15s%d\n", "Samples:", results->samples);

		printf("%-15s%f\n", "Clock (MHz)", CPU_MHZ);
		
		printf("Estimated time for system call using __rdtscp() is %3.3e us\n",
				(float) (1E6 * results->mean / CPU_MHZ));
	}
}

void meas_syscall_gettime(const int num_iterations, const int num_warmup)
{

	struct timeval start_time;
	struct timeval stop_time;
	struct timeval diff_time;

	int fd;
	int loop_cnt = 0;

	fd = open(SAMPLE_FILE, O_CREAT | O_RDONLY, S_IRWXU);
	if (fd == -1) {
		errexit(strerror(errno));
	}

	/* Skip the first few of these */
	while (loop_cnt < num_warmup) {
		read(fd, NULL, 0);
		loop_cnt++;
	}

	/* Start the timer */
	if (gettimeofday(&start_time, NULL) == -1) {
		errexit(strerror(errno));
	}

	/* Now we issue a truck load of system calls */
	while (loop_cnt < num_iterations) {
		read(fd, NULL, 0);
		loop_cnt++;
	}

	/* Stop the timer */
	if (gettimeofday(&stop_time, NULL) == -1) {
		errexit(strerror(errno));
	}
	close(fd);

	/* There are macros to diff timeval structs, see `man 3 timeradd` */
	timersub(&stop_time, &start_time, &diff_time);
	printf("Estimated time for system call using gettimeofday() is %3.3e us\n",
		(float) diff_time.tv_usec / (loop_cnt - num_warmup));
}

void meas_syscall_intrin(const int num_iterations, const int num_warmup)
{

	struct meas_results results;

	int fd;

	unsigned long long before, after;
	unsigned long long delta;

	/* The IA32_TSC_AUX MSR (signature value) is stored here */	
	unsigned int tsc_aux;

	fd = open(SAMPLE_FILE, O_CREAT | O_RDONLY, S_IRWXU);
	if (fd == -1) {
		errexit(strerror(errno));
	}

	results.min = 0;
	results.max = 0;
	results.samples = 0;
	/* Use this member as an accumulator and then divide at the end */
	results.mean = 0;

	for (int i=0; i<num_iterations; i++) {

		/* Measure our system call */
		before = __rdtscp(&tsc_aux);
		read(fd, NULL, 0);
		after = __rdtscp(&tsc_aux);

		/* Ignore the first few measurements to let the cache warmup */
		if (i >= num_warmup) {
			delta = (after - before);
			results.mean += delta;
			if (delta > results.max) {
				results.max = delta;
			}
			if ( (delta < results.min ) && ( delta != 0 ) ) {
				results.min = delta;
			}
			results.samples++;
#ifdef RDTSCP_DEBUG
			fprintf(stderr, "Proc ID: %u\n", tsc_aux);
#endif  /* RDTSCP_DEBUG */
		}
	}
	results.mean = results.mean / results.samples;

	print_results(&results);
	
	close(fd);
}

int main(int argc, char *argv[])
{
	int num_iterations;

	if (argc == 1) {
		num_iterations = NUM_DEFAULT_ITERATIONS;
	} else {
		num_iterations = atoi(argv[1]);
	}

	cpu_set_t mask;

	/* 
	 * Constrain process and its children to only one CPU - affinity mask
	 * for children created via fork() are inherited and mask is preserved
	 * across an execve() calls
	 *
	 * There appear to be accessor functions for CPU masks that are described
	 * in `bits/cpu-set.h` which are also mentioned in the example given in
	 * the man pages for `sched_setaffinity()`.
	 *
	 * Check out `man cpu_set` as well to understand the macros for
	 * manipulating CPU sets
	 */

	CPU_ZERO(&mask);
	CPU_SET(BOUND_CPU, &mask);

	/* Note that this just sets the bits in the mask, it doesn't actually
	 * apply them yet (which is why setting the BOUND_CPU to one that isn't
	 * in the system doesn't generate an error).  Setting a bit in a mask
	 * doesn't require the hardware to do anything, it's just a bit.
	 */
	if (!CPU_ISSET(BOUND_CPU, &mask)) {
		errexit("Could not set CPU %d in mask", BOUND_CPU);
	}

	/* The actual change in processor affinity occurs here */
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
		if (errno == EINVAL) {
			errexit("Bit mask contains no processors currently on the system");
		} else {
			errexit(strerror(errno));
		}
	}
	printf("Processor afinity bound to CPU ID %d\n", BOUND_CPU);

	/* Measure using x86_64 intrinsic */
	meas_syscall_intrin(num_iterations, NUM_WARMUP);
	/* Measure using a less accurate metric */
	meas_syscall_gettime(num_iterations, NUM_WARMUP);

	return 0;
}

