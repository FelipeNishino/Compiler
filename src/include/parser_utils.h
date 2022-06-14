#ifndef COMPILER_PARSER_UTILS_H
#define COMPILER_PARSER_UTILS_H

#include "parser.h"

void parser_get_token(parser* p);
parser parser_make_copy(parser* p);
void parser_return_to(parser* p, parser p2);
void parser_skip_block_or_statement(parser* p);
void parser_skip_to(parser* p, token_type t);
token* parser_find_block_end(parser* p, int should_return);

#endif //COMPILER_PARSER_UTILS_H
