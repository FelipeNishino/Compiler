#include "include/nl.h"
#include "include/iomanager.h"
#include "include/token.h"
#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>

void nl_compile(char* src) {
	parser* p = init_parser(src);

	if (parser_parse(p)) {
		fprintf(stderr, "[Parser.c]: Failed parsing source code\n");
		exit(1);
	}
	free(p);
}

void nl_tokenize_file(const char* filename) {
	char* src = iomanager_get_input_from_file(filename);
	parser* p = init_parser(src);
	token* t = 0;
	do {
		t = lexer_read_token(p->lexer);
		token_print(t);
	}
	while(t->type != token_EOF);
	free(src);
}

void nl_compile_file(const char* filename) {
	char* src = iomanager_get_input_from_file(filename);
	nl_compile(src);
	free(src);
}
