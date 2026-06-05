/*
 * parser.h -- parser
 *
 * Simple enough recursive-descent parser.
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "cons.h"

Value* readexpr(Lexer* l);
void print(Value* v);
void printsexp(Value* v);

#endif /* PARSER_H */
