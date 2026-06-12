/*
 * lexer.c -- lexer
 *
 * DEAD simple tokenizer based on switch. If you do not understand this, you
 * simply cannot code C.
 */

#include <ctype.h>
#include <stdlib.h>

#include "lexer.h"

static void
skipws(Lexer* l)
{
	while (*l->cur && isspace(*l->cur))
		l->cur++;
}

static int
issymch(char c)
{
	return !isspace(c) && c != '(' && c != ')';
}

/*****************************************************************************/

void
lexinit(Lexer* l, const char* src)
{
	l->src = src;
	l->cur = src;
}


Token
nexttok(Lexer* l)
{
	skipws(l);
	Token t = { 0 };

	switch (*l->cur) {
		case '\0':	t.type = TOK_EOF; return t;
		case '.':	l->cur++; t.type = TOK_DOT; return t;
		case '(':	l->cur++; t.type = TOK_LPAREN; return t;
		case ')':	l->cur++; t.type = TOK_RPAREN; return t;
		case '\'':	l->cur++; t.type = TOK_QUOT; return t;
	}

	/* number */
	if (isdigit(*l->cur)) {
		t.type = TOK_NUM;
		t.num = strtod(l->cur, (char**)&l->cur);
		return t;
	}

	/* symbol */
	const char* start = l->cur;
	while (*l->cur && issymch(*l->cur))
		l->cur++;

	t.type  = TOK_SYM;
	t.start = start;
	t.len   = l->cur - start;

	return t;
}

