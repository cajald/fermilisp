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

	{ &(Value){ .type = VAL_SYM, .v.sym = "define" }, bi_define, 1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "if" },     bi_if,     1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "lambda" }, bi_lambda, 1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "quote" },  bi_quote,  1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "set!" },   bi_set,    1 },
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
	Value* body   = cdr(args);

	return mklambda(params, body, env);
}

Value*
bi_define(Env* env, Value* args)
{
	Value* targ = car(args);
	Value* rest = cdr(args);

	/* Variables: (define x 1254) */
	if (targ->type == VAL_SYM) {
		Value* val = eval(env, car(rest));
		defenv(env, targ->v.sym, val);
		return val;
	}

	/* Functions: (define (f x y) body) */
	if (targ->type == VAL_CONS) {
		Value* funname = car(targ);
		Value* params = cdr(targ);
		Value* body = rest;

		/* this is shorthand for: (define funname (lambda (params) body) */
		Value* lambd = mklambda(params, body, env);

		defenv(env, funname->v.sym, lambd);
		return lambd;
	}

	die("malformed define");
	return mknil();
}

Value*
bi_set(Env* env, Value* args)
{
	Value* targ = car(args);
	Value* expr = car(cdr(args));

	if (targ->type != VAL_SYM)
		die("set! requires a symbol");

	Value* val = eval(env, expr);

	if (!envset(env, targ->v.sym, val))
		die("can't set!: unbound variable");

	return val;
}

static int
truth(Value* v)
{
	return !(v->type == VAL_BOOL && v->v.num == 0);
}

Value*
bi_if(Env* env, Value* args)
{
	if (isnil(args)) die("if: missing test");

	Value* test = car(args);

	Value* rest = cdr(args);
	if (isnil(rest)) die("if: missing then branch");

	Value* thenp = car(rest);

	Value* elsep = mknil();
	if (!isnil(cdr(rest)))
		elsep = car(cdr(rest));

	Value* cond = eval(env, test);

	if (!truth(cond))
		return eval(env, elsep);

	return eval(env, thenp);
}

