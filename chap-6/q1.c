#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "defines.h"

#define	NUM_LOOPS	1E6

int main(int argc, char *argv[])
{

	int fd;
	int loop_cnt = 0;

	struct timeval start_time;
	struct timeval stop_time;
	struct timeval diff_time;

	fd = open("sample.txt", O_CREAT | O_RDONLY, S_IRWXU);
	if (fd == -1) {
		errexit(strerror(errno));
	}

	/* Start time */
	if (gettimeofday(&start_time, NULL) == -1) {
		errexit(strerror(errno));
	}
	/* Now we issue a truck load of system calls */
	while (loop_cnt < NUM_LOOPS) {
		read(fd, NULL, 0);
		loop_cnt++;
	}

	/* Stop time */
	if (gettimeofday(&stop_time, NULL) == -1) {
		errexit(strerror(errno));
	}
	close(fd);

	/* There are macros to diff timeval structs, see `man 3 timeradd` */
	timersub(&start_time, &stop_time, &diff_time);
	printf("Estimated time for system call is: %f\n", (float) (diff_time.tv_usec / NUM_LOOPS));

	return 0;
}
