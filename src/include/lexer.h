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
} lexer;

lexer* lexer_init(char* src);
void lexer_next(lexer* lexer);
char lexer_peek(lexer* lexer, int offset);
token* lexer_peek_token(lexer* lexer);
void lexer_skip_whitespace(lexer* lexer);
token* lexer_tokenize(lexer* lexer, token_type type);
token* lexer_n_tokenize(lexer* lexer, int n, token_type type);
token* lexer_read_identifier(lexer* lexer);
token* lexer_read_number_literal(lexer* lexer);
token* lexer_read_string_literal(lexer* lexer);
size_pos* lexer_is_reserved(lexer* lexer);
token* lexer_read_token(lexer* lexer);

#endif //COMPILER_LEXER_H
