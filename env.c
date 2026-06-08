/*
 * env.c -- environments
 *
 * Environments are basically scopes.
 */

#include <string.h>
#include "env.h"
#include "util.h"

Value*
env_lookup(Env* env, const char* name)
{
	while (env) {
		for (Binding* b = env->bindings; b; b = b->next) {
			if (strcmp(b->symname, name) == 0)
				return b->val;
		}
		env = env->parent;
	}

	return NULL;
}

Binding*
mkbind(const char* symname, Value* val, Binding* next)
{
	Binding* b = emalloc(sizeof(*b));

	b->symname = estrndup(symname, 512);
	b->val = val;
	b->next = next;

	return b;
}

Env*
mkenv(Env* parent)
{
	Env* env = emalloc(sizeof(*env));
	env->bindings = NULL;
	env->parent = parent;
	return env;
}

void
defenv(Env* env, const char* name, Value* val)
{
	env->bindings = mkbind(name, val, env->bindings);
}

int
envset(Env* env, const char* name, Value* new)
{
	while (env) {
		for (Binding* b = env->bindings; b; b = b->next) {
			if (strcmp(b->symname, name) == 0) {
				b->val = new;
				return 1;
			}
		}
		env = env->parent;
	}
	return 0;
}

