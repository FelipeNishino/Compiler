#ifndef COMPILER_PARSER_OPERATIONS_H
#define COMPILER_PARSER_OPERATIONS_H

#include "macros.h"
#include "literal.h"
#include "parser.h"

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

boolean parser_operation_comparation(parser* p, literal* lh, literal *rh, operation op);
boolean parser_operation_equality(parser* p, literal* lh, literal *rh, operation op);
literal* parser_operation_or(literal* lh, literal *rh);
literal* parser_operation_and(literal* lh, literal *rh);
literal* parser_operation_not(literal* lh);
literal* parser_operation_additive(literal* lh, literal *rh, operation op);
literal* parser_operation_multiply(literal* lh, literal *rh);
literal* parser_operation_divide(literal* lh, literal *rh, operation op);
literal* parser_operation(parser* p, operation op, literal* lh, literal *rh);

#endif //COMPILER_PARSER_OPERATIONS_H