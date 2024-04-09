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

#define MSG_SIZE	5

static char msg[MSG_SIZE] = "Hello";
static char buf[MSG_SIZE];

int main(int argc, char *argv[])
{
	pid_t wr_cpid;
	pid_t rd_cpid;

	int val;
	/* store the result of failed waitpid() calls */
	int wr_wstatus;
	int rd_wstatus;

	int pipefd[2];

	/* First create the pipe */
	if (pipe(pipefd) == 1) {
		errexit(strerror(errno));
	}

	wr_cpid = fork();
	if (wr_cpid == -1) {
		errexit(strerror(errno));
	} else if (wr_cpid == 0) {
		/* Write child process */
		close(pipefd[0]);
		val = write(pipefd[1], &msg, MSG_SIZE);
		if (val != MSG_SIZE) {
			errexit("Could not write to pipe");
		}
		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	} else {
		/* Parent process */
		rd_cpid = fork();
		if (rd_cpid == -1) {
			errexit(strerror(errno));
		} else if (rd_cpid == 0) {
			/* Read child process */
			close(pipefd[1]);
			read(pipefd[0], &buf, MSG_SIZE);
			close(pipefd[0]);

			write(STDOUT_FILENO, &buf, MSG_SIZE);
			write(STDOUT_FILENO, "\n", 1);

			exit(EXIT_SUCCESS);
		} else {
			/*
			 * See `man 2 wait` for more details, including an
			 * example on status checking
			 */
			if (waitpid(wr_cpid, &wr_wstatus, 0) != wr_cpid) {
				errexit(strerror(errno));
			}
			if (waitpid(rd_cpid, &rd_wstatus, 0) != rd_cpid) {
				errexit(strerror(errno));
			}

			if (WIFEXITED(wr_wstatus)) {
				fprintf(stdout, "Exited normally. Write PID wstatus = 0x%x\n", wr_wstatus);
			} else {
				fprintf(stderr, "Exited abnormally. Write PID wstatus = 0x%x\n", wr_wstatus);
			}

			if (WIFEXITED(rd_wstatus)) {
				fprintf(stdout, "Exited normally. Read PID wstatus = 0x%x\n", rd_wstatus);
			} else {
				fprintf(stderr, "Exited abnormally. Read PID wstatus = 0x%x\n", rd_wstatus);
			}
		}
	}

	return 0;
}

