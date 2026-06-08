/*
 * env.h -- environments
 *
 * Environments are basically scopes.
 */

#ifndef ENV_H
#define ENV_H

#include "cons.h"

typedef struct binding Binding;
typedef struct env     Env;

struct binding
{
	const char* symname;
	Value* val;
	Binding* next;
};

struct env
{
	Binding* bindings;
	Env* parent;
};

Value* env_lookup(Env* env, const char* name);

Binding* mkbind(const char* symname, Value* val, Binding* next);
Env*     mkenv(Env* parent);
void     defenv(Env* env, const char* name, Value* val);

#endif /* ENV_H */
