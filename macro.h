/*
 * macro.c -- macros
 */

#include "cons.h"
#include "env.h"

Value* expand(Env* env, Value* sexp, Value* macro);

