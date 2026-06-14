/*
 * cons.h -- cons cell/list impl
 *
 * A cons list is what LISP uses for representing lists.
 *
 * TODO:
 * 	- list with a custom termination (a . in lisp)
 */

#ifndef CONS_H
#define CONS_H

#include "cons.h"

#include <stdint.h>

typedef struct cons    Cons;
typedef struct lambda  Lambda;
typedef struct lambda  Macro;
typedef struct val     Value;

struct env;

enum valtype
{
	VAL_NIL,
	VAL_CONS,
	VAL_SYM,
	VAL_NUM,
	VAL_LAMBDA,
	VAL_MACRO,
	VAL_BOOL,
};

typedef enum valtype ValType;

struct lambda
{
	Value* params;
	Value* body;
	struct env*   closure;
};

struct cons
{
	Value* car;
	Value* cdr;
};

struct val
{
	ValType type;
	union {
		Cons    cons;
		const char* sym;
		double  num;
		Lambda  lambda;
		Macro   macro;
	} v;
};

/* constructors */
Value* mkbool(int b);
Value* mknil(void);
Value* mknum(double d);
Value* mksym(const char* s);
Value* mklambda(Value* params, Value* body, struct env* closure);
Value* mkmacro(Value* params, Value* body, struct env* closure);
Value* cons(Value* car, Value* cdr);

/* list ops */
Value* car(Value* v);
Value* cdr(Value* v);
int    isnil(Value* v);
int    iscons(Value* v);

#endif /* CONS_H */

