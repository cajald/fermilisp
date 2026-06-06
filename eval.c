/*
 * eval.c -- evaluation
 *
 * Evaluates S-exps
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "util.h"
#include "cons.h"

static Value*
evallist(Value* list)
{
	if (isnil(list))
		return mknil();

	Value* head = NULL;
	Value* tail = NULL;

	while (!isnil(list))
	{
		Value* ev = eval(car(list));

		Value* node = cons(ev, mknil());

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

		list = cdr(list);
	}

	return head;
}

static int
compare(const void* a, const void* b)
{
	const struct builtin* ba = (const struct builtin*)a;
	const struct builtin* bb = (const struct builtin*)b;

	return strcmp(ba->sym->v.sym, bb->sym->v.sym);
}

Value*
eval(Value* sexp)
{
	Value* c = car(sexp);
	Value* args = cdr(sexp);

	if (c->type != VAL_SYM)
		die("invalid function call");

	struct builtin key;
	key.sym = c;

	const char* name = c->v.sym;

	struct builtin key;
	Value tmp;
	tmp.type = VAL_SYM;
	tmp.v.sym = name;

	key.sym = &tmp;

	die("unknown function");
	return mknil();
}

