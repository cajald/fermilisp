/*
 * builtins.c -- builtins
 */

#include <stdio.h>

#include "eval.h"
#include "cons.h"
#include "util.h"
#include "parser.h"

const struct builtin builtins[] =
{
	{ &(Value){ .type = VAL_SYM, .v.sym = "%" },    bi_mod,    0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "*" },    bi_mul,    0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "+" },    bi_plus,   0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "-" },    bi_minus,  0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "/" },    bi_div,    0 },

	{ &(Value){ .type = VAL_SYM, .v.sym = "car" },  bi_car,    0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "cdr" },  bi_cdr,    0 },

	{ &(Value){ .type = VAL_SYM, .v.sym = "defun" }, bi_defun,  1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "lambda" }, bi_lambda, 1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "quote" }, bi_quote,  1 },
};

const size_t builtins_count = ARRSIZE(builtins);

static Value* expectnum(Value* v)
{
	if (v->type != VAL_NUM)
		die("expected number");
	return v;
}

Value*
bi_plus(Env* env, Value* args)
{
	double sum = 0;

	while (!isnil(args)) {
		sum += expectnum(car(args))->v.num;
		args = cdr(args);
	}

	return mknum(sum);
}

Value*
bi_minus(Env* env, Value* args)
{
	double x = expectnum(car(args))->v.num;
	args = cdr(args);

	if (isnil(args))
		return mknum(-x);

	while (!isnil(args)) {
		x -= expectnum(car(args))->v.num;
		args = cdr(args);
	}

	return mknum(x);
}

Value*
bi_mul(Env* env, Value* args)
{
	double r = 1;

	while (!isnil(args)) {
		r *= expectnum(car(args))->v.num;
		args = cdr(args);
	}

	return mknum(r);
}

Value*
bi_div(Env* env, Value* args)
{
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

Value*
bi_mod(Env* env, Value* args)
{
	long a = (long)expectnum(car(args))->v.num;
	long b = (long)expectnum(car(cdr(args)))->v.num;

	if (b == 0) die("mod by zero");

	return mknum(a % b);
}

Value*
bi_car(Env* env, Value* args)
{
	return car(car(args));
}

Value*
bi_cdr(Env* env, Value* args)
{
	return cdr(car(args));
}

Value*
bi_quote(Env* env, Value* args)
{
	return car(args);
}

Value*
bi_lambda(Env* env, Value* args)
{
	Value* params = car(args);
	Value* body   = car(cdr(args));

	return mklambda(params, body, env);
}

Value*
bi_defun(Env* env, Value* args)
{
	die("defun not implemented");
	return mknil();
}

