/*
 * parser.c -- parser
 *
 * Simple enough recursive-descent parser.
 */

#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"
#include "util.h"

static Value*
readatom(Token t)
{
	if (t.type == TOK_NUM)
		return mknum(t.num);

	if (t.type == TOK_SYM)
		return mksym(estrndup(t.start, t.len));

	/* At this point, it is for sure that the atom is malformed */
	die("bad atom");
	return NULL; /* unreachable */
}
/*****************************************************************************/

static Value*
readlist(Lexer* l)
{
	Token t = nexttok(l);

	if (t.type == TOK_RPAREN)
		return mknil();

	Value* head = NULL;
	Value* tail = NULL;

	while (t.type != TOK_RPAREN)
	{
		Value* elem;

		if (t.type == TOK_LPAREN)
			elem = readlist(l);
		else
			elem = readatom(t);

		Value* node = cons(elem, mknil());

		if (!head)
		{
			head = node;
			tail = node;
		}
		else
		{
			tail->v.cons.cdr = node;
			tail = node;
		}

		t = nexttok(l);
	}

	return head;
}

Value*
readexpr(Lexer* l)
{
	Token t = nexttok(l);

	switch (t.type) {
		case TOK_LPAREN:	return readlist(l);
		case TOK_NUM:
		case TOK_SYM:		return readatom(t);
		case TOK_RPAREN:	die("unexpected )");
		case TOK_EOF:           return NULL;
	}

	/* At this point, the token *IS* invalid */
	die("unknown token");
	return NULL;
}

void
printlist(Value* v)
{
	printf("(");
	while (!isnil(v)) {
		print(car(v));
		v = cdr(v);
		if (!isnil(v)) printf(" ");
	}
	printf(")");
}

void
print(Value* v)
{
	switch (v->type) {
		case VAL_NUM:
			printf("%g", v->v.num);
			break;
		case VAL_SYM:
			printf("%s", v->v.sym);
			break;
		case VAL_CONS:
			printlist(v);
			break;
		case VAL_LAMBDA:
			printf("#<lambda at 0x%p>", (void*)v);
		case VAL_NIL:
			printf("()");
			break;
	}
}

