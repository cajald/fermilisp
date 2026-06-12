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
	Value *head = NULL;
	Value *tail = NULL;

	for (;;) {
		Token t = nexttok(l);

		if (t.type == TOK_RPAREN)
			return head ? head : mknil();

		if (t.type == TOK_DOT) {
			if (!tail)
				die("invalid dotted pair (no left side)");

			Value* rhs = readexpr(l);

			Token end = nexttok(l);
			if (end.type != TOK_RPAREN)
				die("expected ')' after dotted pair");

			tail->v.cons.cdr = rhs;
			return head ? head : mknil();
		}

		Value* elem;

		if (t.type == TOK_LPAREN)
			elem = readlist(l);
		else if (t.type == TOK_NUM || t.type == TOK_SYM)
			elem = readatom(t);
		else
			die("unexpected token in list");

		Value* node = cons(elem, mknil());

		if (!head)
			head = tail = node;
		else {
			tail->v.cons.cdr = node;
			tail = node;
		}
	}
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
		case TOK_DOT:           die("unexpected .");
		case TOK_QUOT:		return cons(mksym(estrndup("quote", sizeof("quote"))), cons(readexpr(l), mknil()));
	}

	/* At this point, the token *IS* invalid */
	die("unknown token");
	return NULL;
}

/*****************************************************************************/

void
printlist(Value *v)
{
	printf("(");

	for (;;) {
		print(car(v));

		v = cdr(v);

		if (isnil(v))
			break;

		if (!iscons(v)) {
			printf(" . ");
			print(v);
			break;
		}

		printf(" ");
	}

	printf(")");
}

void
print(Value* v)
{
	switch (v->type) {
		case VAL_NUM:
			printf("%f", v->v.num);
			break;
		case VAL_SYM:
			printf("%s", v->v.sym);
			break;
		case VAL_CONS:
			printlist(v);
			break;
		case VAL_LAMBDA:
			printf("#<lambda at 0x%p>", (void*)v);
			break;
		case VAL_BOOL:
			printf("%s", v->v.num ? "#t" : "#f");
			break;
		case VAL_NIL:
			printf("()");
			break;
	}
}

