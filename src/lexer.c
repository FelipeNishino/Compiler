#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/lexer.h"
#include "include/macros.h"
#include "include/token.h"

Lexer* lexer_init(char* src) {
	Lexer* lexer = (Lexer*) malloc(sizeof(Lexer));
	
	lexer->src = src;
	lexer->src_size = strlen(src);
	lexer->c = lexer->src[0];
	lexer->i = 0;
	lexer->t_pos = (text_position*) malloc(sizeof(text_position));
	lexer->t_pos->line = 1;
	lexer->t_pos->col = 1;

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

Token* lexer_peek_token(Lexer* lexer) {
	Token* t = 0;
	int i, c, line, col;
	i = lexer->i;
	c = lexer->c;
	line = lexer->t_pos->line;
	col = lexer->t_pos->col;

	t = lexer_read_token(lexer);

	lexer->i = i;
	lexer->c = c;
	lexer->t_pos->line = line;
	lexer->t_pos->col = col;

	return t;
}

void lexer_skip_whitespace(Lexer* lexer) {
	int is_whitespace = 1;
	while (is_whitespace && lexer->c != 0) {
		switch (lexer->c) {
			case '\t':
				lexer->t_pos->col += 4;
				break;
			case 10:
				lexer->t_pos->line++;
				lexer->t_pos->col = 1;
				break;
			case 32:
				lexer->t_pos->col++;
				break;
			default:
				is_whitespace = 0;
				continue;
		}
		lexer_next(lexer);
	}
}

Token* lexer_tokenize(Lexer* lexer, TokenType type) {
	return lexer_n_tokenize(lexer, 1, type);
}

Token* lexer_n_tokenize(Lexer* lexer, int n, TokenType type) {
	int i;
	char* val = calloc(n + 1, sizeof(char));
	strncpy(val, &lexer->src[lexer->i], n);
	
	// lexer->t_pos->col += n;
	Token* token = token_init(val, type, lexer->t_pos->line, lexer->t_pos->col);

	if (token->type != token_EOF){
		for (i = 0; i < n; i++)
			lexer_next(lexer);
	}

	return token;
}

void lexer_skip_comment(Lexer* lexer) {
	while (lexer->c != 10 && lexer->c != 0) lexer_next(lexer);
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
	// ([0-9]*)\.([0-9]*)((e)[\-\+][0-9]+)?
	int n = 1;
	char next = lexer_peek(lexer, n);
	int is_float = 0;

	while (isdigit(next) || next == 'e' || next == '.' || next == '-' || next == '+') {
		if (!is_float && next == '.') is_float = 1;

		n++;
		next = lexer_peek(lexer, n);
	}

	return lexer_n_tokenize(lexer, n, (is_float ? token_literal_float : token_literal_int));
}

Token* lexer_read_string_literal(Lexer* lexer) {
	int n = 1;
	char next = lexer_peek(lexer, n);

	// while (isalpha(next) || next == 32 || next == '_' || next == '\"') {
	while (next != '\"') {
		n++;
		next = lexer_peek(lexer, n);
	}

	n++;

	return lexer_n_tokenize(lexer, n, token_literal_string);
}

SizePos* lexer_is_reserved(Lexer* lexer) {
	int i, j = 1, s, n = LEN(RESERVED_WORD_STRING);
	for (i = 0; i < n; i++) {
		if (lexer->c == RESERVED_WORD_STRING[i][0]) {
			s = strlen(RESERVED_WORD_STRING[i]);

			j = 1;
			while (lexer_peek(lexer, j) == RESERVED_WORD_STRING[i][j] && j < s) j++;
			if (j != s) continue;
			return isalnum(lexer_peek(lexer, j)) ? NULL : sizepos_init(i, j);
		}
	}
		
	return NULL;
}

Token* lexer_read_token(Lexer* lexer) {
	// int reserved_word_i, reserved_word_n;

	SizePos* rw_info;

	lexer_skip_whitespace(lexer);
	if (isalpha(lexer->c)) {
		// if ((reserved_word_i = lexer_is_reserved(lexer)) >= 0) {	
			// TODO: retornar o token correto de acordo com a palavra reservada
			if ((rw_info = lexer_is_reserved(lexer))) {
				// if (strcmp(RESERVED_WORD_STRING[reserved_word_i]), "let")
				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "let") == 0)
					// exit(1);
					return lexer_n_tokenize(lexer, rw_info->n, token_LET);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "var") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_VAR);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "if") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_IF);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "else") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_ELSE);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "for") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_FOR);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "while") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_WHILE);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "do") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_DO);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "return") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_RETURN);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "Int") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_type_INT);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "Float") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_type_FLOAT);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "String") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_type_STRING);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "Bool") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_type_BOOL);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "True") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_literal_true);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "False") == 0)
					return lexer_n_tokenize(lexer, rw_info->n, token_literal_false);

				// return lexer_n_tokenize(lexer, reserved_word_n, token_reserved);
			}
		// }
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
		case '&': {
			if (lexer_peek(lexer, 1) == '&') return lexer_n_tokenize(lexer, 2, token_op_AND);
			return lexer_tokenize(lexer, token_UNK);
		}
		case '|': {
			if (lexer_peek(lexer, 1) == '|') return lexer_n_tokenize(lexer, 2, token_op_OR);
			return lexer_tokenize(lexer, token_UNK);
		}
		case '/': {
			if (lexer_peek(lexer, 1) == '/') {
				lexer_skip_comment(lexer);
				return lexer_read_token(lexer);
			}
			// if (lexer_peek(lexer, 1) == '*') {
			// 	lexer_skip_comment(lexer);
			// 	return lexer_read_token(lexer);
			// }
			return lexer_tokenize(lexer, token_op_div);
		}
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
