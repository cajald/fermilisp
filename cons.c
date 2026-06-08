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
#include "env.h"
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
	v->v.sym = s;
	return v;
}

Value*
mklambda(Value* params, Value* body, Env* closure)
{
	Value* v = emalloc(sizeof(*v));
	v->type = VAL_LAMBDA;

	v->v.lambda.params = params;
	v->v.lambda.body = body;
	v->v.lambda.closure = closure;

	return v;
}

Value*
cons(Value* carv, Value* cdrv)
{
	Value* v = emalloc(sizeof(*v));
	v->type = VAL_CONS;
	v->v.cons.car = carv;
	v->v.cons.cdr = cdrv;
	return v;
}

Value*
car(Value* v)
{
	if (v->type != VAL_CONS) die("car on non-cons");
	return v->v.cons.car;
}

Value*
cdr(Value* v)
{
	if (v->type != VAL_CONS) die("cdr on non-cons");
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

