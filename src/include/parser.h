#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "lexer.h"
#include "token.h"
#include "variable.h"
#include "scope.h"
#include "literal.h"
#include "macros.h"
#include "type.h"
#include "str_to_int.h"
#include "str_to_float.h"

typedef struct parser {
	lexer* lexer;
	scope* global_scope;
	token* current_token;
    token* prev_token;
} parser;

parser* init_parser(char* src);
void parser_compare_token_type(parser* p, token_type t1, token_type t2);
int parser_parse(parser* p);
void parser_program(parser* p);
void parser_function(parser* p);
void parser_signature(parser* p);
void parser_argument(parser* p);
type parser_type_annotation(parser* p);
void parser_block(parser* p);
void parser_control_sequence(parser* p, token_type tokenType);
void parser_std_while(parser* p);
void parser_do_while(parser* p);
void parser_for(parser* p);
void parser_if(parser* p);
void parser_else(parser* p, boolean result_if);
boolean parser_condition(parser* p);
void parser_return(parser* p);
void parser_statement(parser* p, int semicolon_override);
void parser_simple_statement(parser* p, int semicolon_override);
variable parser_declaration(parser* p, scope* scope);
int parser_declaration_directive(parser* p);
void parser_assignment(parser* p, scope* scope);
literal* parser_assignment_expression(parser* p, type expected_type);
literal* parser_expression(parser* p);

literal* parser_simple_expression(parser* p);
literal* parser_term(parser* p);
literal* parser_factor(parser* p);
literal* parser_grouped_expression(parser* p);
void parser_unary_operator(parser* p);
void parser_higher_precedence_operator(parser* p);
void parser_lesser_precedence_operator(parser* p);
void parser_relational_operator(parser* p);

literal* parser_string(parser* p);
literal* parser_number(parser* p, int is_int, int is_negative);
literal* parser_integer(parser* p, int is_negative);
literal* parser_float(parser* p, int is_negative);
void parser_higher_precedence_binary_arithmetic_operator(parser* p);
void parser_lesser_precedence_binary_arithmetic_operator(parser* p);
void parser_binary_logical_operator(parser* p);
void parser_higher_precedence_binary_logical_operator(parser* p);
void parser_lesser_precedence_binary_logical_operator(parser* p);
char* parser_identifier(parser* p);
type parser_type(parser* p);

void parser_skip_block_or_statement(parser* p);

#endif //COMPILER_PARSER_H
