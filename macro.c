/*
 * macro.c -- macros
 */

#include "cons.h"
#include "env.h"
#include "eval.h"
#include "util.h"

Value*
bi_defmacro(Env* env, Value* args)
{
	Value* name = car(args);
	Value* params = car(cdr(args));
	Value* body = cdr(cdr(args));
	Value* mac = mkmacro(params, body, env);
	defenv(env, name->v.sym, mac);

	return name;
}

Value*
expand(Env* env, Value* sexp, Value* macro)
{
	Value* params = macro->v.macro.params;
	Value* body = macro->v.macro.body;
	Env* clos = macro->v.macro.closure;

	/* get the args */
	Value* args = cdr(sexp);

	Env* callenv = mkenv(clos);
	while (!isnil(params)) {
		if (isnil(args))
			die("too few arguments to macro");

		Value* param = car(params);
		if (param->type != VAL_SYM)
			die("macro param must be a symbol");

		defenv(callenv,
			param->v.sym,
			car(args));

		params = cdr(params);
		args = cdr(args);
	}

	if (!isnil(args))
		die("too many args to macro");

	Value* exp = mknil();

	while (!isnil(body)) {
		exp = eval(callenv, car(body));
		body = cdr(body);
	}

	return exp;
}

