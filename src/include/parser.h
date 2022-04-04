#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "lexer.h"

typedef struct parser {
	Lexer* lexer;
} Parser;

Parser* init_parser(char* src);
int parser_parse(Parser* parser);

#endif //COMPILER_PARSER_H
