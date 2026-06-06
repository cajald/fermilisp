/*
 * eval.c -- evaluation
 *
 * Evaluates S-exps
 */

#ifndef EVAL_H
#define EVAL_H

#include "cons.h"

typedef Value* (*BuiltinCallback)(Value*);

struct builtin
{
	Value* sym;
	BuiltinCallback cb;
	int special;
};

extern const struct builtin builtins[];
extern const size_t builtins_count;

/* builtins */
Value* bi_plus(Value*);
Value* bi_minus(Value*);
Value* bi_mul(Value*);
Value* bi_div(Value*);
Value* bi_mod(Value*);
Value* bi_car(Value*);
Value* bi_cdr(Value*);
Value* bi_lambda(Value*);
Value* bi_quote(Value*);
Value* bi_defun(Value*);

Value* eval(Value*);

#endif

