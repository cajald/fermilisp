/*
 * lexer.c -- lexer
 *
 * DEAD simple tokenizer based on switch.
 */

#ifndef LEXER_H
#define LEXER_H

enum ttype
{
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_SYM,
	TOK_NUM,
	TOK_EOF,
};

struct tok
{
	enum ttype  type;
	const char* start;
	int         len;
	double      num;
};

struct lex
{
	const char* src;
	const char* cur;
};

typedef enum   ttype TType;
typedef struct tok   Token;
typedef struct lex   Lexer;

void lexinit(Lexer* l, const char* src);
Token nexttok(Lexer* l);

#endif /* LEXER_H */

