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

Boolean parser_operation_comparation(Parser* parser, Literal* lh, Literal *rh, operation op);
Boolean parser_operation_equality(Parser* parser, Literal* lh, Literal *rh, operation op);
Literal* parser_operation_or(Literal* lh, Literal *rh);
Literal* parser_operation_and(Literal* lh, Literal *rh);
Literal* parser_operation_not(Literal* lh);
Literal* parser_operation_additive(Literal* lh, Literal *rh, operation op);
Literal* parser_operation_multiply(Literal* lh, Literal *rh);
Literal* parser_operation_divide(Literal* lh, Literal *rh, operation op);
Literal* parser_operation(Parser* parser, operation op, Literal* lh, Literal *rh);

#endif //COMPILER_PARSER_OPERATIONS_H