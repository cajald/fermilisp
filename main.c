/*
 * main.c -- entry point
 *
 * This only contains the CLI.
 */

#include <stdio.h>
#include <stdlib.h>

#include "arg.h"
#include "eval.h"
#include "env.h"
#include "lexer.h"
#include "parser.h"
#include "repl.h"
#include "util.h"

char* argv0;

int   strict = 0;
char* script = "";
Env*  global = NULL;

extern void feval(Env* env, const char* path);

static char*
readfile(const char* path)
{
	FILE* f = fopen(path, "r");
	if (!f) die("cannot open SCRIPT");

	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	rewind(f);

	char* buf = emalloc(len + 1);
	if (fread(buf, 1, len, f) != (size_t)len)
		die("cannot read file");

	buf[len] = '\0';
	fclose(f);
	return buf;
}

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

	global = mkenv(NULL);

	/* make #t, nil and #f */
	defenv(global, "#t", mkbool(1));
	defenv(global, "#f", mkbool(0));
	defenv(global, "nil", mknil());
	feval(global, "prelude.scm");

	if (argc == 0)
		repl(global);

	script = readfile(argv[0]);

	Lexer lex;
	lexinit(&lex, script);

	while (1)
	{
		Value* expr = readexpr(&lex);

		/* EOF */
		if (expr == NULL || expr->type == VAL_NIL)
			break;

		Value* result = eval(global, expr);

		print(result);
		printf("\n");
	}

	return EXIT_SUCCESS;
}

