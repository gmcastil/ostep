#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/*
 * Write a program that opens a file (with the open() system call) and then
 * calls fork() to create a new process. Can both the child and parent access
 * the file descriptor returned by open()? What happens when they are writing to
 * the file concurrently, i.e., at the same time?
 *
 * So, first of all, both processes are able to communicate to the open file
 * descriptor. And accesses to the open file were both successful, but not in a
 * deterministic order.  It appears that the scheduler lets the parent and child
 * sort of share the resource, but not in a predictable way. The file pointer is
 * just left where it's at and it appears to be up to the user to keep track of
 * everything.  There's some strange behavior with closing which I do not quite
 * understand.
 */

int main(int argc, char *argv[])
{
	int rc;
	int fd;

	fd = open("q2.txt", O_RDWR|O_CREAT);
	if (fd == -1) {
		fprintf(stderr, "Could not open file for writing\n");
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}

	rc = fork();
	if (rc < 0) {
		fprintf(stderr, "Could not fork process\n");
		close(fd);
		exit(1);
	} else if (rc == 0) {
		/* child process */
		printf("Child: open file descriptor %d\n", fd);
		for (int i=0; i<1024; i++) {
			write(fd, "Child wrote\n", 12);
		}
		if (close(fd)) {
			fprintf(stderr, "Child could not close FD %d\n", fd);
		}
		exit(0);
	} else {
		/* parent process */
		printf("Parent: open file descriptor %d\n", fd);
		for (int i=0; i<1024; i++) {
			write(fd, "Parent wrote\n", 13);
		}
		if (close(fd)) {
			fprintf(stderr, "Parent could not close FD %d\n", fd);
		}
		exit(0);
	}

	return 0;
}
