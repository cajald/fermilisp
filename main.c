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

int
main(int argc, char** argv)
{
	ARGBEGIN {
		case 'v':
			printf("fermiLisp version %s\n", VERSION);
			return EXIT_SUCCESS;
		case 'h':
			printf("%s: usage: %s [-v] [-h] [SCRIPT]\n", argv0, argv0);
			return EXIT_SUCCESS;
		default:
			fprintf(stderr, "%s: unknown flag %c\n", argv0, ARGC());
			return EXIT_FAILURE;
	} ARGEND

	if (argc < 3) {
		die("repl is todo");
	}

	return EXIT_SUCCESS;
}

