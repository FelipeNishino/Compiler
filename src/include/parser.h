#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "lexer.h"
#include "token.h"
#include "variable.h"
#include "scope.h"
#include "literal.h"

typedef struct parser {
	Lexer* lexer;
	Scope* globalScope;
} Parser;

Parser* init_parser(char* src);
void parser_compare_token_type(TokenType t1, TokenType t2);
int parser_parse(Parser* parser);
void parser_program(Parser* parser);
void parser_function(Parser* parser);
void parser_signature(Parser* parser);
void parser_argument(Parser* parser);
Type parser_type_annotation(Parser* parser);
void parser_block(Parser* parser);
void parser_control_sequence(Parser* parser, TokenType tokenType);
void parser_std_while(Parser* parser);
void parser_do_while(Parser* parser);
void parser_for(Parser* parser);
void parser_if(Parser* parser);
void parser_else(Parser* parser, int result_if);
int parser_condition(Parser* parser);
void parser_return(Parser* parser);
void parser_literal(Parser* parser);
void parser_statement(Parser* parser);
void parser_declaration(Parser* parser, Scope* scope);
int parser_declaration_directive(Parser* parser);
void parser_assignment(Parser* parser, Scope* scope);
Literal parser_assignment_expression(Parser* parser, Type expected_type);
void parser_simple_expression(Parser* parser);
Literal parser_relational_expression(Parser* parser);
void parser_arithmetic_expression(Parser* parser);
void parser_grouped_arithmetic_expression(Parser* parser);
void parser_operand(Parser* parser);

void parser_binary_arithmetic_operator(Parser* parser);
void parser_unary_arithmetic_operator(Parser* parser);
void parser_unary_logical_operator(Parser* parser);

Literal parser_string(Parser* parser);
void parser_number(Parser* parser);
void parser_integer(Parser* parser);
void parser_float(Parser* parser);
void parser_higher_precedence_binary_arithmetic_operator(Parser* parser);
void parser_lesser_precedence_binary_arithmetic_operator(Parser* parser);
void parser_binary_logical_operator(Parser* parser);
void parser_higher_precedence_binary_logical_operator(Parser* parser);
void parser_lesser_precedence_binary_logical_operator(Parser* parser);
char* parser_identifier(Parser* parser);
Type parser_type(Parser* parser);

void parser_skip_block(Parser* parser);

#endif //COMPILER_PARSER_H
