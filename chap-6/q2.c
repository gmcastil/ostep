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

/* This could be passed in as an argument, but I'm running in a VM with 2 cores */
#define NUM_CPUS	2
/* We wish to constrain our process and its children to the last CPU */
#define BOUND_CPU	(NUM_CPUS - 1)

int main()
{
	unsigned long long rt_val;
	unsigned int tsc;

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
	if (!CPU_ISSET(BOUND_CPU, &mask)) {
		errexit("Could not set CPU %d in mask", BOUND_CPU);
	}
	/* Note that this just sets the bits in the mask, it doesn't actually
	 * apply them yet (which is why setting the BOUND_CPU to one that isn't
	 * in the system doesn't generate an error).  Setting a bit in a mask
	 * doesn't require the hardware to do anything, it's just a bit.
	 */
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
		if (errno == EINVAL) {
			errexit("Bit mask contains no processors currently on the system");
		} else {
			errexit(strerror(errno));
		}
	}

	rt_val = __rdtscp(&tsc);

	printf("RDTSCP register: 0x%llx\n", rt_val);
	printf("Proc ID: %u\n", tsc);

	return 0;
}

