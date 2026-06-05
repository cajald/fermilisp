/*
 * main.c -- entry point
 *
 * This only contains the CLI.
 */

#include <stdio.h>
#include <stdlib.h>

#include "arg.h"
#include "util.h"

char* argv0;

int   strict = 0;
char* script = "";

static void
usage(void)
{
	printf("%s: usage: %s [-v] [-s] [-h] <SCRIPT>\n", argv0, argv0);
}

int
main(int argc, char** argv)
{
	ARGBEGIN {
		case 'v':
			printf("fermiLisp version %s\n", VERSION);
			return EXIT_SUCCESS;
		case 's':
			strict = 1;
			break;
		case 'h':
			usage();
			return EXIT_SUCCESS;
		default:
			fprintf(stderr, "%s: unknown flag %c\n", argv0, ARGC());
			usage();
			return EXIT_FAILURE;
	} ARGEND
	
	if (argc == 0) {
		die("repl is todo");
	}

	script = argv[0];

	return EXIT_SUCCESS;
}

