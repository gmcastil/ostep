#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PROMPT_STR_LEN		256

char *prompt_str;

char *gsh_get_prompt()
{
	static int ret;
	static int prompt_cnt = 0;

	/* Allocate a fixed buffer size for the prompt string */
	if (prompt_cnt == 0) {
		prompt_str = malloc(sizeof(char[PROMPT_STR_LEN]));
		if (prompt_str == NULL) {
			fprintf(stderr, "Null pointer check\n");
			exit(1);
		}
	}

	/* Create a prompt string containing the current working directory */
	ret = snprintf(prompt_str, sizeof(char[PROMPT_STR_LEN]),
			"[%d] %s$ ", prompt_cnt, "cwd");
	if (ret < 0) {
		fprintf(stderr, "String error\n");
		exit(1);
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


