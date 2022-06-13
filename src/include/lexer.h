#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include "token.h"

typedef struct {
	int line;
	int col;
} text_position;

typedef struct {
	char* src;
	size_t src_size;
	char c;
	unsigned int i;
	text_position* t_pos;
} Lexer;

Lexer* lexer_init(char* src);
void lexer_next(Lexer* lexer);
char lexer_peek(Lexer* lexer, int offset);
Token* lexer_peek_token(Lexer* lexer);
void lexer_skip_whitespace(Lexer* lexer);
Token* lexer_tokenize(Lexer* lexer, TokenType type);
Token* lexer_n_tokenize(Lexer* lexer, int n, TokenType type);
Token* lexer_read_identifier(Lexer* lexer);
Token* lexer_read_number_literal(Lexer* lexer);
Token* lexer_read_string_literal(Lexer* lexer);
SizePos* lexer_is_reserved(Lexer* lexer);
Token* lexer_read_token(Lexer* lexer);

#endif //COMPILER_LEXER_H
