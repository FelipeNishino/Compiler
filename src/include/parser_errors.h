#ifndef COMPILER_PARSER_ERRORS_H
#define COMPILER_PARSER_ERRORS_H

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

#include "str_to_int.h"
#include "str_to_float.h"
#include "parser.h"

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

void parser_destroy_all(parser* p);
void parser_raise_error(parser* p, parse_error e, const char* err_str_arg, ...);
void parser_assert_type(parser* p, type t1, type t2);
void parser_assert_token_type(parser* p, token_type t1, token_type t2);
void parser_assert_number_conversion(parser* p, str2num_errno res_code);

#endif //COMPILER_PARSER_ERRORS_H
