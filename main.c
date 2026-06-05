/*
 * main.c -- entry point
 *
 * This only contains the CLI.
 */

#include <stdio.h>
#include <stdlib.h>

#include "arg.h"
#include "lexer.h"
#include "util.h"

char* argv0;

int   strict = 0;
char* script = "";

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
	
	if (argc == 0) {
		die("repl is todo");
	}

	script = readfile(argv[0]);
	Lexer lex;
	lexinit(&lex, script);

	for (;;) {
		Token t = nexttok(&lex);
		switch (t.type) {
			case TOK_EOF:
				return EXIT_SUCCESS;
			case TOK_NUM:
				printf("%f (num)\n", t.num);
				break;
			case TOK_SYM:
				printf("%.*s (sym)\n", t.len, t.start);
				break;
			case TOK_LPAREN:
				puts("LPAREN");
				break;
			case TOK_RPAREN:
				puts("RPAREN");
				break;
		}
	}

	return EXIT_SUCCESS;
}

