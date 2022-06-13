#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "lexer.h"
#include "token.h"
#include "variable.h"
#include "scope.h"
#include "literal.h"
#include "macros.h"
#include "str_to_int.h"
#include "str_to_float.h"

typedef struct parser {
	Lexer* lexer;
	Scope* global_scope;
	Token* current_token;
    Token* prev_token;
} Parser;

typedef enum {
    illegal_assignment_constant,
    invalid_int_literal,
    invalid_float_literal,
    invalid_operation,
    invalid_operation_mod,
    invalid_operation_pow,
    invalid_operation_unary,
	type_mismatch,
    undefined_error,
    undefined_identifier,
    unexpected_eof,
    unexpected_token,
    variable_redeclaration,
} parse_error;

typedef struct {
    enum { DU_INT, DU_FLOAT } discriminant;
    union
    {
        str2int_errno i_errno;
        str2float_errno f_errno;
    };
} str2num_errno;

#define ERR_ILLEGAL_ASSIGNMENT_CONSTANT "Illegal assignment to constant variable \"%s\""
#define ERR_INVALID_INT_LITERAL "Invalid Int literal {%s}"
#define ERR_INVALID_FLOAT_LITERAL "Invalid Float literal {%s}"
#define ERR_INVALID_OPERATION "Invalid operator %s for %s and %s"
#define ERR_INVALID_OPERATION_MOD "Modulo operation expects Int operands, got %s and %s"
#define ERR_INVALID_OPERATION_POW "Exponent should be an Int"
#define ERR_INVALID_OPERATION_UNARY "Can´t apply unary operator %s to %s type"
#define ERR_TYPE_MISMATCH "Type mismatch between {%s} and {%s}"
#define ERR_UNDEFINED_ERROR ""
#define ERR_UNDEFINED_IDENTIFIER "Identifier {%s} undefined in current scope"
#define ERR_UNEXPECTED_EOF "Unexpected end-of-file while parsing"
#define ERR_UNEXPECTED_TOKEN "Unexpected token {%s}, expected {%s}"
#define ERR_VARIABLE_REDECLARATION "Variable redeclaration for identifier {%s}"

#define FOREACH_OP(OP) \
        OP(op_sum)   \
        OP(op_subtraction)   \
        OP(op_multi)   \
        OP(op_pow)   \
        OP(op_div)   \
        OP(op_mod)   \
        OP(op_not)   \
        OP(op_and)   \
        OP(op_or)   \
        OP(op_lt)   \
        OP(op_lte)   \
        OP(op_eq)   \
        OP(op_neq)   \
        OP(op_gt)   \
        OP(op_gte)   \

typedef enum {
    FOREACH_OP(GENERATE_ENUM)
} operation;

//https://stackoverflow.com/questions/8312168/static-const-char-defined-but-not-used
static const char * const OP_STRING[] = {
    FOREACH_OP(GENERATE_STRING)
};

Parser* init_parser(char* src);
void parser_compare_token_type(Parser* parser, TokenType t1, TokenType t2);
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
void parser_statement(Parser* parser, int semicolon_override);
Variable parser_declaration(Parser* parser, Scope* scope);
int parser_declaration_directive(Parser* parser);
void parser_assignment(Parser* parser, Scope* scope);
Literal* parser_assignment_expression(Parser* parser, Type expected_type);
Literal* parser_expression(Parser* parser);

Literal* parser_arithmetic_expression(Parser* parser);
Literal* parser_term(Parser* parser);
Literal* parser_factor(Parser* parser);
Literal* parser_grouped_expression(Parser* parser);
void parser_unary_operator(Parser* parser);
void parser_higher_precedence_operator(Parser* parser);
void parser_lesser_precedence_operator(Parser* parser);
void parser_relational_operator(Parser* parser);

Literal* parser_string(Parser* parser);
Literal* parser_number(Parser* parser, int is_int, int is_negative);
Literal* parser_integer(Parser* parser, int is_negative);
Literal* parser_float(Parser* parser, int is_negative);
void parser_higher_precedence_binary_arithmetic_operator(Parser* parser);
void parser_lesser_precedence_binary_arithmetic_operator(Parser* parser);
void parser_binary_logical_operator(Parser* parser);
void parser_higher_precedence_binary_logical_operator(Parser* parser);
void parser_lesser_precedence_binary_logical_operator(Parser* parser);
char* parser_identifier(Parser* parser);
Type parser_type(Parser* parser);

void parser_skip_block_or_statement(Parser* parser);

#endif //COMPILER_PARSER_H
