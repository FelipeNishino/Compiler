#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/lexer.h"
#include "include/macros.h"
#include "include/token.h"

lexer* lexer_init(char* src) {
	lexer* l = (lexer*) malloc(sizeof(lexer));
	
	l->src = src;
	l->src_size = strlen(src);
	l->c = l->src[0];
	l->i = 0;
	l->t_pos = (text_position*) malloc(sizeof(text_position));
	l->t_pos->line = 1;
	l->t_pos->col = 1;

	return l;
}

void lexer_next(lexer* l) {
	if (l->i < l->src_size) {
		l->i++;
		l->t_pos->col++;
		l->c = l->src[l->i];
	}
}

char lexer_peek(lexer* l, int offset) {
	return l->src[offset > 0 ? MAX(l->i + offset, 0) : MIN(l->i + offset, l->src_size)];	
}

token* lexer_peek_token(lexer* l) {
	token* t = 0;
	int i, c, line, col;
	i = l->i;
	c = l->c;
	line = l->t_pos->line;
	col = l->t_pos->col;

	t = lexer_read_token(l);

	l->i = i;
	l->c = c;
	l->t_pos->line = line;
	l->t_pos->col = col;

	return t;
}

void lexer_skip_whitespace(lexer* l) {
	int is_whitespace = 1;
	while (is_whitespace && l->c != 0) {
		switch (l->c) {
			case '\t':
				l->t_pos->col += 4;
				break;
			case 10:
				l->t_pos->line++;
				l->t_pos->col = 1;
				break;
			case 32:
				l->t_pos->col++;
				break;
			default:
				is_whitespace = 0;
				continue;
		}
		lexer_next(l);
	}
}

token* lexer_tokenize(lexer* l, token_type t_type) {
	return lexer_n_tokenize(l, 1, t_type);
}

token* lexer_n_tokenize(lexer* l, int n, token_type t_type) {
	int i;
	char* val = calloc(n + 1, sizeof(char));
	strncpy(val, &l->src[l->i], n);
	
	// l->t_pos->col += n;
	token* token = token_init(val, t_type, l->t_pos->line, l->t_pos->col);

	if (token->type != token_EOF){
		for (i = 0; i < n; i++)
			lexer_next(l);
	}

	return token;
}

void lexer_skip_comment(lexer* l) {
	while (l->c != 10 && l->c != 0) lexer_next(l);
}

token* lexer_read_identifier(lexer* l) {
	int n = 1;	
	char next = lexer_peek(l, n);

	while (isalnum(next)) {
		n++;
		next = lexer_peek(l, n);
	}

	return lexer_n_tokenize(l, n, token_identifier);
}

token* lexer_read_number_literal(lexer* l) {
	// ([0-9]*)\.([0-9]*)((e)[\-\+][0-9]+)?
	int n = 1;
	char next = lexer_peek(l, n);
	int is_float = l->c == '.';

	while (isdigit(next) || next == 'e' || next == '.' || (l->src[l->i + n-1] == 'e' && (next == '-' || next == '+'))) {
		if (!is_float && next == '.') is_float = 1;
		// if (!is_exponent && next == 'e') is_exponent = 1;
		n++;
		next = lexer_peek(l, n);
	}

	return lexer_n_tokenize(l, n, (is_float ? token_literal_float : token_literal_int));
}

token* lexer_read_string_literal(lexer* l) {
	int n = 1;
	char next = lexer_peek(l, n);

	// while (isalpha(next) || next == 32 || next == '_' || next == '\"') {
	while (next != '\"') {
		n++;
		next = lexer_peek(l, n);
	}

	n++;

	return lexer_n_tokenize(l, n, token_literal_string);
}

size_pos* lexer_is_reserved(lexer* l) {
	int i, j = 1, s, n = LEN(RESERVED_WORD_STRING);
	for (i = 0; i < n; i++) {
		if (l->c == RESERVED_WORD_STRING[i][0]) {
			s = strlen(RESERVED_WORD_STRING[i]);

			j = 1;
			while (lexer_peek(l, j) == RESERVED_WORD_STRING[i][j] && j < s) j++;
			if (j != s) continue;
			return isalnum(lexer_peek(l, j)) ? NULL : sizepos_init(i, j);
		}
	}
		
	return NULL;
}

token* lexer_read_token(lexer* l) {
	// int reserved_word_i, reserved_word_n;

	size_pos* rw_info;

	lexer_skip_whitespace(l);
	if (isalpha(l->c)) {
		// if ((reserved_word_i = lexer_is_reserved(l)) >= 0) {	
			// TODO: retornar o token correto de acordo com a palavra reservada
			if ((rw_info = lexer_is_reserved(l))) {
				// if (strcmp(RESERVED_WORD_STRING[reserved_word_i]), "let")
				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "let") == 0)
					// exit(1);
					return lexer_n_tokenize(l, rw_info->n, token_LET);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "var") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_VAR);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "if") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_IF);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "else") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_ELSE);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "for") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_FOR);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "while") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_WHILE);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "do") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_DO);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "return") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_RETURN);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "Int") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_type_INT);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "Float") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_type_FLOAT);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "String") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_type_STRING);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "Bool") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_type_BOOL);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "True") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_literal_true);

				if (strcmp(RESERVED_WORD_STRING[rw_info->i], "False") == 0)
					return lexer_n_tokenize(l, rw_info->n, token_literal_false);

				// return lexer_n_tokenize(l, reserved_word_n, token_reserved);
			}
		// }
		return lexer_read_identifier(l);
	}

	if (isdigit(l->c))
		return lexer_read_number_literal(l);		

	switch(l->c) {
		case '.': return lexer_read_number_literal(l);
		case '+': return lexer_tokenize(l, token_op_plus);
		case '-': {
			if (lexer_peek(l, 1) == '>') return lexer_n_tokenize(l, 2, token_op_arrow);
			return lexer_tokenize(l, token_op_minus);
		}
		case '%': return lexer_tokenize(l, token_op_mod); 
		case '&': {
			if (lexer_peek(l, 1) == '&') return lexer_n_tokenize(l, 2, token_op_AND);
			return lexer_tokenize(l, token_UNK);
		}
		case '|': {
			if (lexer_peek(l, 1) == '|') return lexer_n_tokenize(l, 2, token_op_OR);
			return lexer_tokenize(l, token_UNK);
		}
		case '/': {
			if (lexer_peek(l, 1) == '/') {
				lexer_skip_comment(l);
				return lexer_read_token(l);
			}
			// if (lexer_peek(l, 1) == '*') {
			// 	lexer_skip_comment(l);
			// 	return lexer_read_token(l);
			// }
			return lexer_tokenize(l, token_op_div);
		}
		case '=': {
			if (lexer_peek(l, 1) == '=') return lexer_n_tokenize(l, 2, token_op_eq);
			return lexer_tokenize(l, token_op_assignment);
		}
		case '<': {
			if (lexer_peek(l, 1) == '=') return lexer_n_tokenize(l, 2, token_op_lte);
			return lexer_tokenize(l, token_op_lt);
		}
		case '>': {
			if (lexer_peek(l, 1) == '=') return lexer_n_tokenize(l, 2, token_op_gte);
			return lexer_tokenize(l, token_op_gt);
		}
		case '!': {
			if (lexer_peek(l, 1) == '=') return lexer_n_tokenize(l, 2, token_op_neq);
			return lexer_tokenize(l, token_op_NOT);
		}
		case '*': {
			if (lexer_peek(l, 1) == '*') return lexer_n_tokenize(l, 2, token_op_pow);
			return lexer_tokenize(l, token_op_multi);
		}
		case '"': return lexer_read_string_literal(l);
		case ':': return lexer_tokenize(l, token_op_colon);
		case ';': return lexer_tokenize(l, token_gp_semicolon);
		case '(': return lexer_tokenize(l, token_gp_op);
		case ')': return lexer_tokenize(l, token_gp_cp);
		case '[': return lexer_tokenize(l, token_gp_osb);
		case ']': return lexer_tokenize(l, token_gp_csb);
		case '{': return lexer_tokenize(l, token_gp_ocb);
		case '}': return lexer_tokenize(l, token_gp_ccb);
		case '\0': return lexer_tokenize(l, token_EOF);
		default: fprintf(stderr, "[Lexer.c]: unexpected symbol {%c}, value {%d}\n", l->c, l->c); exit(1);
	}
}
