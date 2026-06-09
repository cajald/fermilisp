/*
 * eval.c -- evaluation
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "cons.h"
#include "util.h"
#include "env.h"

static Value* evallist(Env* env, Value* list);

static int
compare(const void* a, const void* b)
{
	const struct builtin* ba = a;
	const struct builtin* bb = b;

	return strcmp(ba->sym->v.sym, bb->sym->v.sym);
}

static Value*
lambd_apply(Env* env, Value* fn, Value* args)
{
	Value* params = fn->v.lambda.params;
	Value* body   = fn->v.lambda.body;
	Env* closure   = fn->v.lambda.closure;

	Value* evaled_args = evallist(env, args);
	Env* call_env = mkenv(closure);

	while (!isnil(params)) {
		if (isnil(evaled_args))
			die("too few arguments");

		Value *param = car(params);

		if (param->type != VAL_SYM)
			die("lambda parameter shall be a symbol");

		defenv(call_env,
				param->v.sym,
				car(evaled_args));

		params = cdr(params);
		evaled_args = cdr(evaled_args);
	}

	if (!isnil(evaled_args))
		die("too many arguments");

	Value* res = mknil();

	while (!isnil(body)) {
		res = eval(call_env, car(body));
		body = cdr(body);
	}
	return res;
}

static Value*
evallist(Env* env, Value* list)
{
	if (isnil(list))
		return mknil();

	Value* head = NULL;
	Value* tail = NULL;

	while (!isnil(list)) {
		Value* ev = eval(env, car(list));
		Value* node = cons(ev, mknil());

		if (!head)
			head = tail = node;
		else {
			tail->v.cons.cdr = node;
			tail = node;
		}

		list = cdr(list);
	}

	return head;
}

Value*
eval(Env* env, Value* sexp)
{
	switch (sexp->type) {
	case VAL_NUM:
	case VAL_NIL:
	case VAL_LAMBDA:
		return sexp;
	case VAL_SYM: {
		/* first try env */
		Value* v = env_lookup(env, sexp->v.sym);

		if (v)
			return v;

		struct builtin key;
		key.sym = sexp;

		/* if not, try builtins */
		struct builtin* found = bsearch(
			&key,
			builtins,
			builtins_count,
			sizeof(struct builtin),
			compare
		);

		if (found)
			return found->sym;  /* return canonical symbol */

		die("undefined symbol");
	}
	case VAL_BOOL:
		return sexp;
	case VAL_CONS: {
		      Value* op_node = car(sexp);
		      Value* args    = cdr(sexp);

		      Value* op = eval(env, op_node);

		      if (op->type == VAL_LAMBDA)
			      return lambd_apply(env, op, args);

		      if (op->type == VAL_SYM) {
			      struct builtin key;
			      key.sym = op;

			      struct builtin* found = bsearch(
					      &key,
					      builtins,
					      builtins_count,
					      sizeof(struct builtin),
					      compare
					      );

			      if (!found)
				      die("unknown function");

			      Value* evaluated_args =
				      found->special ? args : evallist(env, args);

			      return found->cb(env, evaluated_args);
		      }

		      die("not callable");
		      return NULL;
	}
	}

	__builtin_unreachable();
}

