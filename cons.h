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

#include <stdint.h>

typedef struct cons     Cons;
typedef struct val      Value;

enum valtype
{
	VAL_NIL,
	VAL_CONS,
	VAL_SYM,
	VAL_NUM,
};

typedef enum   valtype  ValType;

struct cons
{
	Value* car;
	Value* cdr;
};

struct val
{
	ValType type;
	union {
		Cons        cons;
		const char* sym;
		double      num;
		int         nil;
	} v;
};

/*****************************************************************************/

Value* mknil(void);
Value* mknum(double d);
Value* mksym(const char* s);
Value* cons(Value* car, Value* cdr);

Value* car(Value* v);
Value* cdr(Value* v);
int    isnil(Value* v);
int    iscons(Value* v);

#endif /* CONS_H */

