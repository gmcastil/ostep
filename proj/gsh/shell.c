#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "prompt.h"

int main(void)
{

	char *line;

	/* Main loop of the shell */
	while (1) {

		line = readline(gsh_get_prompt());
		if (line == NULL) {
			free(line);
			gsh_prompt_cleanup();
			return 0;
		}
		if (line != NULL) {
			free(line);
		}
	}

	return 0;
}

