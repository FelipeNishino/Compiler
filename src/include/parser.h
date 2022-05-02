#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "lexer.h"

typedef struct parser {
	Lexer* lexer;
} Parser;

Parser* init_parser(char* src);
int parser_parse(Parser* parser);
int parser_program(Parser* parser);
int parser_function(Parser* parser);
int parser_signature(Parser* parser);
int parser_argument(Parser* parser);
int parser_type_annotation(Parser* parser);
int parser_block(Parser* parser);
int parser_control_sequence(Parser* parser);
int parser_std_while(Parser* parser);
int parser_do_while(Parser* parser);
int parser_for(Parser* parser);
int parser_if(Parser* parser);
int parser_else(Parser* parser);
int parser_condition(Parser* parser);
int parser_return(Parser* parser);
int parser_literal(Parser* parser);
int parser_statement(Parser* parser);
int parser_declaration(Parser* parser);
int parser_declaration_directive(Parser* parser);
int parser_assignment(Parser* parser);
int parser_assignment_expression(Parser* parser);
int parser_simple_expression(Parser* parser);
int parser_relational_expression(Parser* parser);
int parser_arithmetic_expression(Parser* parser);
int parser_grouped_arithmetic_expression(Parser* parser);
int parser_operand(Parser* parser);

int parser_binary_arithmetic_operator(Parser* parser);
int parser_unary_arithmetic_operator(Parser* parser);
int parser_unary_logical_operator(Parser* parser);

int parser_string(Parser* parser);
int parser_number(Parser* parser);
int parser_integer(Parser* parser);
int parser_float(Parser* parser);
int parser_higher_precedence_binary_arithmetic_operator(Parser* parser);
int parser_lesser_precedence_binary_arithmetic_operator(Parser* parser);
int parser_binary_logical_operator(Parser* parser);
int parser_higher_precedence_binary_logical_operator(Parser* parser);
int parser_lesser_precedence_binary_logical_operator(Parser* parser);
int parser_identifier(Parser* parser);
int parser_type(Parser* parser);

#endif //COMPILER_PARSER_H
