#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/lexer.h"
#include "include/macros.h"
#include "include/token.h"

Lexer* init_lexer(char* src) {
	Lexer* lexer = calloc(1, sizeof(Lexer));
	
	lexer->src = src;
	lexer->src_size = strlen(src);
	lexer->c = lexer->src[0];
	lexer->i = 0;
	lexer->line = 1;
	lexer->col = 1;

	return lexer;
}

void lexer_next(Lexer* lexer) {
	if (lexer->i < lexer->src_size) {
		lexer->i++;
		lexer->c = lexer->src[lexer->i];
	}
}

char lexer_peek(Lexer* lexer, int offset) {
	return lexer->src[offset > 0 ? MAX(lexer->i + offset, 0) : MIN(lexer->i + offset, lexer->src_size)];	
}

void lexer_skip_whitespace(Lexer* lexer) {
	int is_whitespace = 1;
	while (is_whitespace) {
		switch (lexer->c) {
			case 9:
				lexer->col += 4;
			case 10:
				lexer->line++;
				lexer->col = 1;
				break;
			case 32:
				lexer->col++;
				break;
			default:
				is_whitespace = 0;
				continue;
		}
		lexer_next(lexer);
	}
}

Token* lexer_tokenize(Lexer* lexer, TokenType type) {
	printf("Chegou aqui, vai tokenizar %s\n", TOKEN_TYPE_STRING[type]);
	return lexer_n_tokenize(lexer, 1, type);
}

Token* lexer_n_tokenize(Lexer* lexer, int n, TokenType type) {
	int i;
	char* val = calloc(n + 1, sizeof(char));
	strncpy(val, &lexer->src[lexer->i], n);

	Token* token = init_token(val, type, lexer->line, lexer->col);

	if (token->type != token_EOF){
		for (i = 0; i < n; i++)
			lexer_next(lexer);
	}

	return token;
}

Token* lexer_read_identifier(Lexer* lexer) {
	int n = 1;	
	char next = lexer_peek(lexer, n);

	while (isalnum(next)) {
		n++;
		next = lexer_peek(lexer, n);
	}

	return lexer_n_tokenize(lexer, n, token_identifier);
}

Token* lexer_read_number_literal(Lexer* lexer) {
	int n = 1;
	char next = lexer_peek(lexer, n);

	while (isdigit(next) || next == 'e' || next == '.' || next == '-' || next == '+') {
		n++;
		next = lexer_peek(lexer, n);
	}

	return lexer_n_tokenize(lexer, n, token_literal_number);
}

Token* lexer_read_string_literal(Lexer* lexer) {
	int n = 1;
	char next = lexer_peek(lexer, n);

	while (isalpha(next) || next == 32) {
		n++;
		next = lexer_peek(lexer, n);
	}

	return lexer_n_tokenize(lexer, n, token_literal_string);
}

int lexer_is_reserved(Lexer* lexer) {
	int i, n = LEN(RESERVED_WORD_STRING);
	for (i = 0; i < n; i++)
		if (lexer->c == RESERVED_WORD_STRING[i][0]) return i;
	return -1;
}

int lexer_check_reserved(Lexer* lexer, int word_index) {
	int i, control = 0, n = strlen(RESERVED_WORD_STRING[word_index]);
	for (i = 1; i <= n; i++) {
		control &= lexer_peek(lexer, i) == RESERVED_WORD_STRING[word_index][i]; 
	}

	return (control && !isalnum(lexer_peek(lexer, n + 1))) ? n : 0;
}

Token* lexer_read_token(Lexer* lexer) {
	int reserved_word_i, reserved_word_n;

	lexer_skip_whitespace(lexer);
	if (isalpha(lexer->c)) {
		if ((reserved_word_i = lexer_is_reserved(lexer)) >= 0) {	
			// TODO: retornar o token correto de acordo com a palavra reservada
			if ((reserved_word_n = lexer_check_reserved(lexer, reserved_word_i))) {
				// if (strcmp(RESERVED_WORD_STRING[reserved_word_i]), "let")
				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "let"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_LET);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "var"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_VAR);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "if"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_IF);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "else"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_ELSE);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "for"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_FOR);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "while"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_WHILE);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "do"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_DO);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "return"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_RETURN);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "Int"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_TYPE_INT);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "Float"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_TYPE_FLOAT);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "String"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_TYPE_STRING);

				if (strcmp(RESERVED_WORD_STRING[reserved_word_i], "Bool"))
					return lexer_n_tokenize(lexer, reserved_word_n, token_TYPE_BOOL);

				// return lexer_n_tokenize(lexer, reserved_word_n, token_reserved);
			}
		}
		return lexer_read_identifier(lexer);
	}

	if (isdigit(lexer->c))
		return lexer_read_number_literal(lexer);		

	switch(lexer->c) {
		case '.': return lexer_read_number_literal(lexer);
		case '+': return lexer_tokenize(lexer, token_op_plus);
		case '-': {
			if (lexer_peek(lexer, 1) == '>') return lexer_n_tokenize(lexer, 2, token_op_arrow);
			return lexer_tokenize(lexer, token_op_minus);
		}
		case '%': return lexer_tokenize(lexer, token_op_mod); 
		case '/': return lexer_tokenize(lexer, token_op_div);
		case '=': {
			if (lexer_peek(lexer, 1) == '=') return lexer_n_tokenize(lexer, 2, token_op_eq);
			return lexer_tokenize(lexer, token_op_assignment);
		}
		case '<': {
			if (lexer_peek(lexer, 1) == '=') return lexer_n_tokenize(lexer, 2, token_op_lte);
			return lexer_tokenize(lexer, token_op_lt);
		}
		case '>': {
			if (lexer_peek(lexer, 1) == '=') return lexer_n_tokenize(lexer, 2, token_op_gte);
			return lexer_tokenize(lexer, token_op_gt);
		}
		case '!': {
			if (lexer_peek(lexer, 1) == '=') return lexer_n_tokenize(lexer, 2, token_op_neq);
			return lexer_tokenize(lexer, token_op_NOT);
		}
		case '*': {
			if (lexer_peek(lexer, 1) == '*') return lexer_n_tokenize(lexer, 2, token_op_pow);
			return lexer_tokenize(lexer, token_op_multi);
		}
		case '"': return lexer_read_string_literal(lexer);
		case ':': return lexer_tokenize(lexer, token_op_colon);
		case ';': return lexer_tokenize(lexer, token_gp_semicolon);
		case '(': return lexer_tokenize(lexer, token_gp_op);
		case ')': return lexer_tokenize(lexer, token_gp_cp);
		case '[': return lexer_tokenize(lexer, token_gp_osb);
		case ']': return lexer_tokenize(lexer, token_gp_csb);
		case '{': return lexer_tokenize(lexer, token_gp_ocb);
		case '}': return lexer_tokenize(lexer, token_gp_ccb);
		case '\0': return lexer_tokenize(lexer, token_EOF);
		default: fprintf(stderr, "[Lexer.c]: unexpected symbol {%c}, value {%d}\n", lexer->c, lexer->c); exit(1);
	}
}
