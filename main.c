/*
 * main.c -- entry point
 *
 * This only contains the CLI.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
bool  pmode;
Env*  global = NULL;

extern void feval(Env* env, const char* path);

static char *
readfile(const char* path)
{
	FILE* f = fopen(path, "r");
    char chunk[4096];
    char *buf = NULL;
    size_t len = 0;
    size_t cap = 0;

    while (fgets(chunk, sizeof chunk, f) != NULL) {
        size_t n = strlen(chunk);

        if (len + n + 1 > cap) {
            size_t newcap = cap ? cap : sizeof chunk;

            while (newcap < len + n + 1) {
                if (newcap > (size_t)-1 / 2) {
                    free(buf);
                    return NULL;
                }
                newcap *= 2;
            }

            char *tmp = realloc(buf, newcap);
            if (tmp == NULL) {
                free(buf);
                return NULL;
            }

            buf = tmp;
            cap = newcap;
        }

        memcpy(buf + len, chunk, n);
        len += n;
    }

    if (ferror(f)) {
        free(buf);
        return NULL;
    }

    if (buf == NULL) {
        buf = malloc(1);
        if (buf == NULL)
            return NULL;
    }

    buf[len] = '\0';
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

		case 'p':
			pmode = 1;
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
	defenv(global, "#t", mkbool(true));
	defenv(global, "#f", mkbool(false));
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
		Value* result = eval(global, expr);

		print(result);
		printf("\n");
	}

	return EXIT_SUCCESS;
}

