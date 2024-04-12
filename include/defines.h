#include <stdlib.h>

#define errexit(msg, ...) \
	do { \
		fprintf(stderr, "Error: "); \
		fprintf(stderr, msg, ##__VA_ARGS__); \
		fprintf(stderr, "\n"); \
		exit(EXIT_FAILURE); \
	} while (0)

