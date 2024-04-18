#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *prompt_str;

char *gsh_get_prompt()
{
	static int ret;
	static int prompt_cnt = 0;

	/* Need places to store temporary strings */
	static char *cwd_buf = NULL;
	static char *prompt_cnt_buf = NULL;

	/* Length of the [xxxx] string used in the prompt */
	static int prompt_str_len = 0;

	size_t prompt_str_len = 0;

	/* Free the previously allocated string buffer */
	if (!prompt_str) {
		free(prompt_str);
	}

	/* Get the current working directory */
	cwd_buf = getcwd(NULL, 0);
	if (!cwd_buf) {
		fprintf(stderr, "Null pointer error\n");
		exit(1);
	}
	/* Start our length with the length of the current working directory */
	prompt_str_len = strlen(cwd_buf);

	/* Now add the length of additional characters */
	prompt_cnt_len = snprintf(prompt_cnt_buf, sizeof(prompt_cnt_buf),
			"[%d]", prompt_cnt);
	if (prompt_cnt_len 
	prompt_str_len += strlen("[%d]", prompt_cnt)

	/* Create a prompt string containing the current working directory */
	ret = snprintf(prompt_str, sizeof(char[PROMPT_STR_LEN]),
			"[%d] %s$ ", prompt_cnt, cwd);
	if (ret < 0) {
		fprintf(stderr, "String error\n");
		exit(1);
	}

	/*
	 * Need two more entries for the trailing ' $' it contains, and a third
	 * for the NUL character
	 */
	prompt_str_len += 3;

	/* Per the man page, this can be freed now */
	if (cwd) {
		free(cwd);
	}
	prompt_cnt++;
	return prompt_str;
}

void gsh_prompt_cleanup()
{
	if (prompt_str) {
		free(prompt_str);
	}
}


