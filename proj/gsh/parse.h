/* Command and arguments as understood after parsing */
struct cmd {
	char *prog;
	int prog_argc;
	char *prog_argv[];
};

void parse_line(struct cmd *cmd, char *line);
