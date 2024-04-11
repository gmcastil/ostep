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

int main(int argc, char *argv[])
{
	unsigned __int64 rt_val;
	unsigned int tsc;

	rt_val = __rdtscp(&tsc);
	printf("RDTSCP register: 0x%16x\n", rt_val);

	return 0;
}

