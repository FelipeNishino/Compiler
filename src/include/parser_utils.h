#ifndef COMPILER_PARSER_UTILS_H
#define COMPILER_PARSER_UTILS_H

#include "parser.h"

void parser_get_token(Parser* parser);
Parser parser_make_copy(Parser* parser);
void parser_return_to(Parser* parser, Parser p2);
void parser_skip_block_or_statement(Parser* parser);
void parser_skip_to(Parser* parser, TokenType t);
Token* parser_find_block_end(Parser* parser, int should_return);

#endif //COMPILER_PARSER_UTILS_H
