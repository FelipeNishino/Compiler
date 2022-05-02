#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include "token.h"

typedef struct lexer {
	char* src;
	size_t src_size;
	char c;
	unsigned int i;
	unsigned int line;
	unsigned int col;
} Lexer;

Lexer* init_lexer(char* src);
void lexer_next(Lexer* lexer);
char lexer_peek(Lexer* lexer, int offset);
void lexer_skip_whitespace(Lexer* lexer);
Token* lexer_tokenize(Lexer* lexer, TokenType type);
Token* lexer_n_tokenize(Lexer* lexer, int n, TokenType type);
Token* lexer_read_identifier(Lexer* lexer);
Token* lexer_read_number_literal(Lexer* lexer);
Token* lexer_read_string_literal(Lexer* lexer);
int lexer_is_reserved(Lexer* lexer);
int lexer_check_reserved(Lexer* lexer, int word_index);
Token* lexer_read_token(Lexer* lexer);

#endif //COMPILER_LEXER_H
