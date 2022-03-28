// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/lexer.h"
#include "include/macros.h"
#include "include/token.h"
// #include "include/dfa.h"

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
	return lexer_n_tokenize(lexer, 1, type);
}

Token* lexer_n_tokenize(Lexer* lexer, int n, TokenType type) {
	int i;
	char* val = calloc(n + 1, sizeof(char));
	strncpy(val, &lexer->src[lexer->i], n);

	Token* token = init_token(val, type, lexer->line, lexer->col);
	for (i = 0; i < n; i++)
		lexer_next(lexer);

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

Token* lexer_read_literal(Lexer* lexer) {
	int n = 1;
	char next = lexer_peek(lexer, n);

	// printf("checking literal, next {%c}\n", next);
	while (isdigit(next) || next == 'e' || next == 'E' || next == '.' || next == '-' || next == '+') {
		n++;
		next = lexer_peek(lexer, n);
		// printf("checking literal, next {%c}\n", next);
	}

	return lexer_n_tokenize(lexer, n, token_literal);
}

int lexer_is_reserved(Lexer* lexer) {
	int i, n = LEN(RESERVED_WORDS);
	for (i = 0; i < n; i++)
		if (lexer->c == RESERVED_WORDS[i][0]) return i;
	return -1;
}

int lexer_check_reserved(Lexer* lexer, int word_index) {
	int i, control = 0, n = strlen(RESERVED_WORDS[word_index]);
	for (i = 1; i <= n; i++) {
		control &= lexer_peek(lexer, i) == RESERVED_WORDS[word_index][i]; 
	}

	return (control && !isalnum(lexer_peek(lexer, n + 1))) ? n : 0;
}

Token* lexer_read_token(Lexer* lexer) {
	int reserved_word_i, reserved_word_n;

	lexer_skip_whitespace(lexer);
	if (isalpha(lexer->c)) {
		if ((reserved_word_i = lexer_is_reserved(lexer)) >= 0)
		{	
			if ((reserved_word_n = lexer_check_reserved(lexer, reserved_word_i)))
				return lexer_n_tokenize(lexer, reserved_word_n, token_reserved);
		}
		return lexer_read_identifier(lexer);
	}

	if (isdigit(lexer->c))
		return lexer_read_literal(lexer);		

	switch(lexer->c) {
		case '.': return lexer_read_literal(lexer);
		case '+':
		case '-':
		case '/':
		case '=': return lexer_tokenize(lexer, token_operator);
		case '*': {
			if (lexer_peek(lexer, 1) == '*') return lexer_n_tokenize(lexer, 2, token_operator);
			return lexer_tokenize(lexer, token_operator);
		}
		case ';':
		case '(':
		case ')':
		case '{':
		case '}': return lexer_tokenize(lexer, token_grouper);
		case '\0': return lexer_tokenize(lexer, token_EOF);
		default: printf("[Lexer.c]: unexpecter symbol {%c}, value {%d}\n", lexer->c, lexer->c); exit(1);
	}
}

// TokenNode* tokenize(char* input, DFA *machine) {
// 	int i, j, line = 1, col = 1, tokenStart = 0,inputSize = strlen(input) + 1;
// 	char currentChar = 0, *auxStr = calloc(2, sizeof(char));
// 	Transition currentTransition;
// 	TokenNode *tokens = NULL;
// 	State currentState, state0;
//     state0 = dfa_find(machine, 0)->state;
//     currentState = state0;
//     printf("Got state {%d}, isTerminal {%d}\n", currentState.id, currentState.isTerminal);

//     printf("Initializing tokenization\n");
// 	for (i = 0; i < inputSize; i++) {
// 		currentChar = input[i];

//         printf("Analyzing char {%c}\n", currentChar);        	

// 		if (currentChar == 10) {
//             printf("Is newline, jumping to next char\n");
// 			line++;
// 			col = 1;
// 			continue;
// 		}

//         printf("Searching for valid transition\n");
// 		for (j = 0; j < N_TRANSITIONS; j++) {
// 			currentTransition = DEFAULT_TRANSITIONS[j];
//             printf("Testing transition {%d}; src: {%d}, chars {%s}, target {%d}\n", j, currentTransition.source, currentTransition.read, currentTransition.target);
// 			if (currentTransition.source == currentState.id && strchr(currentTransition.read, currentChar)) {
//                 printf("Is valid! Retrieving state\n");
// 				currentState = dfa_find(machine, currentTransition.target)->state;

// //                if (currentState) {
//                     printf("Got state {%d}, isTerminal {%d}\n", currentState.id, currentState.isTerminal);
// //                }
// //                else {
// //                    printf("Couldn't find desired state on machine\n");
// //                }

// 				if(currentState.isTerminal) {
// 					printf("i {%d}, tokenStart {%d}, newSize {%d}\n", i, tokenStart, (i - tokenStart + 1));
// 					auxStr = realloc(auxStr, i - tokenStart + 1);
// 					printf("Successfully reallocated auxStr");
// 					strncpy(auxStr, input + tokenStart, i - tokenStart);
// 					printf("token encontrado {%s}\n", auxStr);
// 					tokens = tokenList_insertFirst(tokens, (Token) {
// 						.token = auxStr,
// 						.type = currentState.id,
// 						.line = line,
// 						.col = col
// 						});
// 					currentState = state0;
// 					tokenStart = col;
// 					col++;
// 				}
//                 break;
// 			}
// 		}
// 		col++;
// 	}

// 	return tokens;
// }
