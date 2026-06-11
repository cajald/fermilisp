/*
 * builtins.c -- builtins
 */

#include <stdio.h>
#include <string.h>

#include "eval.h"
#include "cons.h"
#include "util.h"
#include "parser.h"

#ifndef NDEBUG
static void
checkbuiltintbl(void)
{
	for (size_t i = 1; i < builtins_count; i++) {
		if (strcmp(
			builtins[i - 1].sym->v.sym,
			builtins[i].sym->v.sym) > 0)
			die("builtins.def not sorted");
	}
}
#endif

const struct builtin builtins[] =
{
#define X(name, fn, special) \
	{ &(Value){ .type = VAL_SYM, .v.sym = name }, fn, special },

#include "builtins.def"

#undef X
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

void
feval(Env* env, const char* path)
{
#	ifndef NDEBUG
	checkbuiltintbl();
#	endif

	FILE* f = fopen(path, "r");
	if (!f) die("require: cannot open file");

	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	rewind(f);

	char* buf = emalloc(len + 1);
	if (fread(buf, 1, len, f) != (size_t)len)
		die("require: read error");

	buf[len] = '\0';
	fclose(f);

	Lexer lex;
	lexinit(&lex, buf);

	while (1) {
		Value* expr = readexpr(&lex);
		if (!expr) break;
		eval(env, expr);
	}

	efree(buf);
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

Value*
bi_require(Env* env, Value* args)
{
	Value* v = car(args);

	if (v->type != VAL_SYM)
		die("require expects string-like symbol");

	feval(env, v->v.sym);
	return mknil();
}

Value*
bi_begin(Env* env, Value* args)
{
	Value* result = mknil();

	while (!isnil(args)) {
		result = eval(env, car(args));
		args = cdr(args);
	}

	return result;
}

Value*
bi_cons(Env* env, Value* args)
{
	if (isnil(args))
		die("cons got too few args");

	Value* rest = car(args);

	if (isnil(rest))
		die("cons got too few args");

	if (!isnil(cdr(rest)))
		die("cons: too many arguments");

	return cons(car(args), car(rest));
}

Value*
bi_list(Env* env, Value* args)
{
	return args; /* and that's it */
}

Value*
bi_apply(Env* env, Value* args)
{
	Value* fn = car(args);
	Value* lst = car(cdr(args));
	Value* expr = cons(fn, lst);

	return eval(env, expr);
}

