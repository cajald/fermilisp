/*
 * builtins.c -- builtins
 */

#include <stdio.h>
#include <string.h>

#include "eval.h"
#include "cons.h"
#include "util.h"
#include "parser.h"

const struct builtin builtins[] =
{
	/* arithmetic */
	{ &(Value){ .type = VAL_SYM, .v.sym = "%" },    bi_mod,    0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "*" },    bi_mul,    0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "+" },    bi_plus,   0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "-" },    bi_minus,  0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "/" },    bi_div,    0 },

	/* comparisons */
	{ &(Value){ .type = VAL_SYM, .v.sym = "<" },    bi_lt,     0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "=" },    bi_eq,     0 },

	/* list ops */
	{ &(Value){ .type = VAL_SYM, .v.sym = "car" },  bi_car,    0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "cdr" },  bi_cdr,    0 },

	/* core forms */
	{ &(Value){ .type = VAL_SYM, .v.sym = "and" },    bi_and,    1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "boolean?" }, bi_booleanp, 0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "define" }, bi_define, 1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "if" },     bi_if,     1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "lambda" }, bi_lambda, 1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "null?" },  bi_nullp,  0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "number?" },bi_numberp,0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "not" },    bi_not,    0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "or" },     bi_or,     1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "pair?" },  bi_pairp,  0 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "quote" },  bi_quote,  1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "set!" },   bi_set,    1 },
	{ &(Value){ .type = VAL_SYM, .v.sym = "symbol?" },bi_symbolp,0 },
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

static int
truth(Value* v)
{
	/* only #f is false */
	return !(v->type == VAL_BOOL && v->v.num == 0);
}

/*****************************************************************************/

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
bi_nullp(Env* env, Value* args)
{
	return mkbool(isnil(car(args)));
}

Value*
bi_pairp(Env* env, Value* args)
{
	return mkbool(iscons(car(args)));
}

Value*
bi_symbolp(Env* env, Value* args)
{
	return mkbool(car(args)->type == VAL_SYM);
}

Value*
bi_numberp(Env* env, Value* args)
{
	return mkbool(car(args)->type == VAL_NUM);
}

Value*
bi_booleanp(Env* env, Value* args)
{
	return mkbool(car(args)->type == VAL_BOOL);
}

Value*
bi_and(Env* env, Value* args)
{
	Value* last = mkbool(1);

	while (!isnil(args)) {
		last = eval(env, car(args));

		if (!truth(last))
			return mkbool(0);

		args = cdr(args);
	}

	return last;
}

Value*
bi_or(Env* env, Value* args)
{
	while (!isnil(args)) {
		Value* v = eval(env, car(args));

		if (truth(v))
			return mkbool(1);

		args = cdr(args);
	}

	return mkbool(0);
}

Value*
bi_not(Env* env, Value* args)
{
	return mkbool(!truth(car(args)));
}

Value*
bi_lt(Env* env, Value* args)
{
	double a = expectnum(car(args))->v.num;
	double b = expectnum(car(cdr(args)))->v.num;

	return mkbool(a < b);
}

Value*
bi_eq(Env* env, Value* args)
{
	Value* a = car(args);
	Value* b = car(cdr(args));

	if (a->type != b->type)
		return mkbool(0);

	switch (a->type) {
		case VAL_NUM:
			return mkbool(a->v.num == b->v.num);

		case VAL_SYM:
			return mkbool(strcmp(a->v.sym, b->v.sym) == 0);

		case VAL_BOOL:
			return mkbool(a->v.num == b->v.num);

		case VAL_NIL:
			return mkbool(1);

		default:
			return mkbool(a == b);
	}
}

Value*
bi_define(Env* env, Value* args)
{
	Value* targ = car(args);
	Value* rest = cdr(args);

	/* (define x expr) */
	if (targ->type == VAL_SYM) {
		Value* val = eval(env, car(rest));
		defenv(env, targ->v.sym, val);
		return val;
	}

	/* (define (f x y) body...) */
	if (targ->type == VAL_CONS) {
		Value* funname = car(targ);
		Value* params  = cdr(targ);
		Value* body    = rest;

		Value* lambda = mklambda(params, body, env);

		defenv(env, funname->v.sym, lambda);
		return lambda;
	}

	die("malformed define");
	return mknil();
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
bi_set(Env* env, Value* args)
{
	Value* sym = car(args);
	Value* expr = car(cdr(args));

	if (sym->type != VAL_SYM)
		die("set! requires a symbol");

	Value* val = eval(env, expr);

	if (!envset(env, sym->v.sym, val))
		die("set!: unbound variable");

	return val;
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

