/*
 * eval.c -- evaluation
 *
 * Evaluates S-exps
 */

#ifndef EVAL_H
#define EVAL_H

#include <stddef.h>

#include "cons.h"
#include "env.h"

typedef Value* (*BuiltinCallback)(Env*, Value*);

struct builtin
{
	Value* sym;
	BuiltinCallback cb;
	int special;
};

extern const struct builtin builtins[];
extern const size_t builtins_count;

Value* bi_plus(Env*, Value*);
Value* bi_minus(Env*, Value*);
Value* bi_mul(Env*, Value*);
Value* bi_div(Env*, Value*);
Value* bi_mod(Env*, Value*);
Value* bi_car(Env*, Value*);
Value* bi_cdr(Env*, Value*);
Value* bi_lambda(Env*, Value*);
Value* bi_quote(Env*, Value*);
Value* bi_define(Env*, Value*);
Value* bi_set(Env*, Value*);
Value* bi_if(Env*, Value*);
Value* bi_lt(Env*, Value*);
Value* bi_eq(Env*, Value*);
Value* bi_nullp(Env*, Value*);
Value* bi_symbolp(Env*, Value*);
Value* bi_pairp(Env*, Value*);
Value* bi_numberp(Env*, Value*);
Value* bi_booleanp(Env*, Value*);
Value* bi_and(Env*, Value*);
Value* bi_or(Env*, Value*);
Value* bi_not(Env*, Value*);

Value* eval(Env* env, Value* sexp);

#endif

