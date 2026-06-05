/*
 * cons.c -- cons cell/list impl
 *
 * A cons list is what LISP uses for representing lists.
 *
 * TODO:
 * 	- terminated lists (ended with nil, like on proper lisp)
 * 	- dotted lists (custom termination)
 */

#include <stdlib.h>

#include "cons.h"
#include "util.h"

Value*
mknil(void)
{
	static Value nil = { .type = VAL_NIL };
	return &nil;
}

Value*
mknum(double d)
{
	Value* v = emalloc(sizeof(*v));
	v->type = VAL_NUM;
	v->v.num = d;
	return v;
}

Value*
mksym(const char* s)
{
	Value* v = emalloc(sizeof(*v));
	v->type = VAL_SYM;
	v->v.sym = s; /* do not copy, just reown */
	return v;
}

Value*
cons(Value* car, Value* cdr)
{
	Value* v = emalloc(sizeof(*v));
	v->type = VAL_CONS;
	v->v.cons.car = car;
	v->v.cons.cdr = cdr;
	return v;
}

/*****************************************************************************/

Value*
car(Value* v)
{
	if (v->type != VAL_CONS)
		die("CAR on non-list");

	return v->v.cons.car;
}

Value*
cdr(Value* v)
{
	if (v->type != VAL_CONS)
		die("CDR on non-list");

	return v->v.cons.cdr;
}

int
isnil(Value* v)
{
	return v->type == VAL_NIL;
}

int
iscons(Value* v)
{
	return v->type == VAL_CONS;
}

