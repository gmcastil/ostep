#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/* If this is not enough stack space for temp storage you have problems */
#define MAX_PROMPT_CWD_LEN		256

static char *gsh_prompt_str;

char *gsh_get_prompt(void)
{
	int ret;
	size_t gsh_prompt_str_size;

	static int gsh_prompt_cnt = 0;
	static char gsh_prompt_cwd_buf[MAX_PROMPT_CWD_LEN];

	int gsh_prompt_len = 0;

	/* This nay have been previously allocated, so free it to reuse the pointer */
	free(gsh_prompt_str);
	
	if (getcwd(gsh_prompt_cwd_buf, sizeof(gsh_prompt_cwd_buf)) == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}
	/* Now calculate the length required for the full prompt string */
	gsh_prompt_len = snprintf(NULL, 0, "[%d] %s\n$ ", gsh_prompt_cnt, gsh_prompt_cwd_buf);
	if (gsh_prompt_len < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}

	/* Allocate and construct the final prompt string */
	gsh_prompt_str_size = (1 + gsh_prompt_len) * sizeof(char);
	gsh_prompt_str = malloc(gsh_prompt_str_size);
	if (!gsh_prompt_str) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}
	ret = snprintf(gsh_prompt_str, gsh_prompt_str_size,
			"[%d] %s\n$ ", gsh_prompt_cnt, gsh_prompt_cwd_buf);
	if (ret > (int) gsh_prompt_str_size) {
		fprintf(stderr, "Prompt string buffer not large enough\n");
		exit(1);
	} else if (ret < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}

	gsh_prompt_cnt++;
	return gsh_prompt_str;
}

void gsh_prompt_cleanup(void)
{
	if (gsh_prompt_str) {
		free(gsh_prompt_str);
	}
}


