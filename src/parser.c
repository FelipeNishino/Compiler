#include "include/parser.h"
#include "include/lexer.h"
#include <stdlib.h>

Parser* init_parser(char* src) {
	Parser* parser = calloc(1, sizeof(Parser));
	parser->lexer = init_lexer(src);

	return parser;
}

int parser_parse(Parser* parser) {
	Token* token = 0;

	do {
		token = lexer_read_token(parser->lexer);
	}
	while (token->type != token_EOF);
	
	printf("%s\n", token_to_str(token));

	return 1;
}