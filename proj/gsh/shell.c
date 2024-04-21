#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "msgs.h"
#include "prompt.h"
#include "parse.h"

int main(void)
{

	char *line;

	struct cmd cmd;

	/* Main loop of the shell */
	while (1) {

		line = readline(gsh_get_prompt());
		if (line == NULL) {

			/* Clean up before exiting */
			free(line);
			gsh_prompt_cleanup();
			return 0;
		}

		/* Parse a line into a command */
		parse_cmd(&cmd, line);
		if (cmd.prog) {
			printf("Entered program %s\n", cmd.prog);
			printf("%d\n", cmd.prog_argc);
			for (int i=0; i<cmd.prog_argc; i++) {
				printf("%s\n", cmd.prog_argv[i]);
			}
		}

		/* If the command is valid, fork and exec the command, else an error  */

		/* This always has to be done before the next iteration */
		free(line);
	}

	/* 
	 * NOTE: it might be more straightforward to handle some of the resource
	 * deallocation in the loop and then use a goto to here to do the final
	 * cleanup that is common regardless of how we exit the shell (e.g.,
	 * freeing the memory allocated to the cmd struct)
	 */

	return 0;
}

