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
	Value* c;
	Value* args;
	Value* evaluated_args;
	Value tmp;
	struct builtin key;
	struct builtin* found;

	if (sexp->type == VAL_NUM || sexp->type == VAL_NIL)
		return sexp;
	
	/* TODO: When variables are implemented, remove this! */
	if (sexp->type == VAL_SYM)
		return sexp;

	c = car(sexp);
	args = cdr(sexp);

	if (c->type != VAL_SYM)
		die("invalid function call");

	tmp.type = VAL_SYM;
	tmp.v.sym = c->v.sym;

	key.sym = &tmp;
	key.cb = NULL;

	found = bsearch(
			&key,
			builtins,
			builtins_count,
			sizeof(struct builtin),
			compare);

	if (!found)
		die("unknown function");

	if (!found->special)
		evaluated_args = evallist(args);
	else
		evaluated_args = args;

	return found->cb(evaluated_args);
}

