/*
 * repl.c -- interactive REPL
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "eval.h"
#include "cons.h"
#include "util.h"

#define REPL_CHUNK 1024

static int
pardepth(const char *s)
{
	int depth = 0;

	while (*s)
	{
		switch (*s)
		{
		case '(':
			depth++;
			break;

		case ')':
			depth--;
			break;
		}

		s++;
	}

	return depth;
}

static char *
readexprline(void)
{
	char buf[REPL_CHUNK];

	char  *src   = NULL;
	size_t len   = 0;
	int    depth = 0;
	int    first = 1;

	for (;;)
	{
		fputs(first ? "> " : ">> ", stdout);
		fflush(stdout);

		if (fgets(buf, sizeof(buf), stdin) == NULL)
		{
			/*
			 * EOF with no pending input:
			 * terminate REPL.
			 */
			if (len == 0)
				return NULL;

			/*
			 * EOF in middle of expression.
			 * Treat what we have as final input.
			 */
			break;
		}

		buf[strcspn(buf, "\n")] = '\0';

		if (strcmp(buf, ",quit") == 0)
			exit(EXIT_SUCCESS);

		if (strcmp(buf, ",l") == 0) {
			FILE* fptr = fopen("LICENSE", "r");

			if (!fptr)
				exit(EXIT_FAILURE);

			char lbuf[REPL_CHUNK];
			while (fgets(lbuf, sizeof(lbuf), fptr) != NULL)
				printf("%s", lbuf);

			fclose(fptr);
			continue;
		}

		if (strcmp(buf, ",w") == 0) {
			printf("\n"
"Affirmer offers the Work as-is and makes no representations or "
"warranties of any kind concerning the Work, express, implied, "
"statutory or otherwise, including without limitation warranties of "
"title, merchantability, fitness for a particular purpose, non "
"infringement, or the absence of latent or other defects, accuracy, or "
"the present or absence of errors, whether or not discoverable, all to "
"the greatest extent permissible under applicable law.\n");
			continue;
		}


		size_t n = strlen(buf);

		src = erealloc(src, len + n + 1);

		memcpy(src + len, buf, n + 1);
		len += n;

		depth += pardepth(buf);

		if (depth <= 0)
			break;

		first = 0;
	}

	if (!src)
	{
		src = emalloc(1);
		src[0] = '\0';
	}

	return src;
}

void
repl(Env* glob)
{
	puts("fermiLisp REPL\n");
	puts("fermiLisp comes with no warranty, use ,w to see warranty "
		"information, ,l to see the license, and ,quit to exit.");

	for (;;) {
		char *input = readexprline();

		if (input == NULL) {
			printf("\n");
			break;
		}

		if (*input == '\0' || strcmp(input, "\n") == 0) {
			efree(input);
			continue;
		}

		Lexer lex;
		lexinit(&lex, input);

		Value *expr = readexpr(&lex);

		if (expr != NULL) {
			Value *result = eval(glob, expr);

			print(result);
			putchar('\n');
		}

		efree(input);
	}
}

