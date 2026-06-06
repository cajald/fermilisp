/*
 * builtins.c -- builtins
 */

#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "eval.h"
#include "cons.h"

/*
 * Builtins must be sorted lexicographically by symbol
 *
 * because we used bsearch() i guess
 */
const struct builtin builtins[] =
{
	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "*" },      .cb = bi_mul },
	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "+" },      .cb = bi_plus },
	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "-" },      .cb = bi_minus },
	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "/" },      .cb = bi_div },

	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "car" },    .cb = bi_car },
	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "cdr" },    .cb = bi_cdr },

	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "defun" },  .cb = bi_defun },
	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "eval" },   .cb = eval },

	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "lambda" }, .cb = bi_lambda },
	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "mod" },    .cb = bi_mod },

	{ .sym = &(Value){ .type = VAL_SYM, .v.sym = "quote" },  .cb = bi_quote },
};

const size_t builtins_count = ARRSIZE(builtins);

/*****************************************************************************/

static Value*
expectnum(Value* v)
{
	if (v->type != VAL_NUM)
		die("expected number");
	return v;
}

/*
static Value*
expectcons(Value* v)
{
	if (v->type != VAL_CONS)
		die("expected cons");
	return v;
}
*/

Value*
bi_plus(Value* args)
{
	double sum = 0;

	while (!isnil(args))
	{
		sum += expectnum(car(args))->v.num;
		args = cdr(args);
	}
	
	return mknum(sum);
}

Value*
bi_minus(Value* args)
{
	if (isnil(args))
		die("expected at least 1 arg for -");

	double x = expectnum(car(args))->v.num;
	args = cdr(args);

	/* negate */
	if (isnil(args))
		return mknum(-x);

	while (!isnil(args))
	{
		x -= expectnum(car(args))->v.num;
		args = cdr(args);
	}

	return mknum(x);
}

Value*
bi_mul(Value* args)
{
	double res = 1;

	while (!isnil(args))
	{
		res *= expectnum(car(args))->v.num;
		args = cdr(args);
	}

	return mknum(res);
}

Value*
bi_div(Value* args)
{
	if (isnil(args))
		die("expected at least 1 arg for /");

	double x = expectnum(car(args))->v.num;
	args = cdr(args);

	if (isnil(args))
		return mknum(1.0 / x);

	while (!isnil(args)) {
		x /= expectnum(car(args))->v.num;
		args = cdr(args);
	}

	return mknum(x);
}

Value* bi_mod(Value* args)
{
	Value* a = expectnum(car(args));
	Value* b = expectnum(car(cdr(args)));

	long x = (long)a->v.num;
	long y = (long)b->v.num;

	if (y == 0)
		die("mod by zero");

	return mknum(x % y);
}

/*****************************************************************************/

Value*
bi_car(Value* args)
{
	return car(car(args));
}

Value*
bi_cdr(Value* args)
{
	return cdr(car(args));
}

/*****************************************************************************/

Value*
bi_quote(Value* args)
{
	return car(args);
}

Value*
bi_lambda(Value* args)
{
	die("lambda is todo :(");
	return mknil();
}

Value* bi_defun(Value* args)
{
	die("defun is todo :(");
	return mknil();
}

