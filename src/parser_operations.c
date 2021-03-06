#include "include/parser.h"
#include "include/parser_operations.h"
#include "include/parser_errors.h"
#include "include/literal.h"
#include "include/type.h"
#include "include/macros.h"
#include <stdlib.h>
#include <string.h>

boolean parser_operation_equality(parser* p, literal* lh, literal *rh, operation op) {
    if (type_compare(lh->type, String) ^ type_compare(rh->type, String))
        parser_raise_error(p, type_mismatch, NULL, TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
    if (type_compare(lh->type, String))
        return (strcmp((char*) lh->value, (char*) rh->value) != 0 ? False : True);
    if (type_compare(lh->type, Bool) ^ type_compare(rh->type, Bool))
        parser_raise_error(p, type_mismatch, NULL, TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
    if (type_compare(lh->type, Bool))
        return bool_init_from_int(*(boolean*)lh->value == *(boolean*)rh->value);
    return bool_init_from_int((lh->type == Int ? *(int*)lh->value : *(float*)lh->value) == (rh->type == Int ? *(int*)rh->value : *(float*)rh->value));
}

boolean parser_operation_comparation(parser* p, literal* lh, literal *rh, operation op) { 
    if (!type_is_number(lh->type)) parser_raise_error(p, invalid_operation, NULL, OP_STRING[op], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
    if (!type_is_number(rh->type)) parser_raise_error(p, invalid_operation, NULL, OP_STRING[op], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
    int res;
    switch (op) {
        case op_lt:
            res = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) < (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
            break;
        case op_lte:
            res = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) <= (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
            break;
        case op_gt:
            res = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) > (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
            break;
        case op_gte:
            res = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) >= (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
            break;
        default: break;
    }
    return bool_init_from_int(res);
}

literal* parser_operation_or(literal* lh, literal *rh) {
    number l = (lh->type == Int ? number_init_with_int(*(int*)lh->value) : number_init_with_float(*(float*)lh->value));
    number r = (rh->type == Int ? number_init_with_int(*(int*)rh->value) : number_init_with_float(*(float*)rh->value));

    lh->value = literal_init_with_bool(number_is(l, I) ? l.i : l.f || number_is(r, I) ? r.i : r.f?True:False)->value;
    return lh;
}

literal* parser_operation_and(literal* lh, literal *rh) {
    number l = (lh->type == Int ? number_init_with_int(*(int*)lh->value) : number_init_with_float(*(float*)lh->value));
    number r = (rh->type == Int ? number_init_with_int(*(int*)rh->value) : number_init_with_float(*(float*)rh->value));

    lh->value = literal_init_with_bool(number_is(l, I) ? l.i : l.f && number_is(r, I) ? r.i : r.f?True:False)->value;
    return lh;
}

literal* parser_operation_not(literal* lh) {
    lh->value = (*(boolean*)lh->value == True ? literal_init_with_bool(False)->value : literal_init_with_bool(True)->value);
    return lh;
}

literal* parser_operation_additive(literal* lh, literal *rh, operation op) {
    int* i = (int*) malloc(sizeof(int));
    float* f = (float*) malloc(sizeof(float));

    if (lh->type == Int) {
        if (op == op_sum)
            *i = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) + (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
        else
            *i = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) - (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
    }
    else {
        if (op == op_sum)
            *f = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) + (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
        else
            *f = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) - (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
    }

    if (lh->type == Int)
        lh->value = i;
    else
        lh->value = f;
    return lh;
}

literal* parser_operation_multiply(literal* lh, literal *rh) {
    int* i = (int*) malloc(sizeof(int));
    float* f = (float*) malloc(sizeof(float));

    if (lh->type == Int) 
        *i = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) * (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
    else
        *f = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) * (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);

    if (lh->type == Int)
        lh->value = i;
    else
        lh->value = f;
    return lh;
}

literal* parser_operation_divide(literal* lh, literal *rh, operation op) {
    int* i = (int*) malloc(sizeof(int));
    float* f = (float*) malloc(sizeof(float));

    if (op == op_div) {
        if (lh->type == Int) 
            *i = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) / (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
        else
            *f = (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) / (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);    
    }
    else
        *i = *(int*)lh->value % *(int*)rh->value;
    
    if (lh->type == Int)
        lh->value = i;
    else
        lh->value = f;

    return lh;
}

literal* parser_operation_power(literal* lh, literal *rh) {
    // TODO: N valeu a pena usar essa union, repensar dps
    number l = (lh->type == Int ? number_init_with_int(*(int*)lh->value) : number_init_with_float(*(float*)lh->value));
    number r = number_init_with_int(*(int*)rh->value);
    number b = l;
    
    // if (ACCESS_NUMBER(r) == 0) return ;
    if (r.i == 1) return lh;
    
    for (int i = 1; i < ABS(r.i); i++) {
        if (number_is(l, I)) {
            if (number_is(b, I))
                l.i *= b.i;
            else
                l.i *= b.f;
        }
        else {
            if (number_is(b, I))
                l.f *= b.i;
            else
                l.f *= b.f;
        }
    }

    literal *result;

    if (number_is(l, I))
        result = literal_init_with_int(l.i);
    else result = literal_init_with_float(l.f);

    if (r.i > 0) return result;

    result->type = Float;
    float *f = (float*) malloc(sizeof(float));
    if (number_is(l, I)) *f = 1 / l.i;
    else if (l.f > 0) *f = 1 / l.f;
    result->value = f;
    return result;
}

literal* parser_operation(parser* p, operation op, literal* lh, literal *rh) {
    if (op < 6) {
        if (!type_is_number(lh->type)) parser_raise_error(p, invalid_operation, NULL, OP_STRING[op], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
        if (!type_is_number(rh->type)) parser_raise_error(p, invalid_operation, NULL, OP_STRING[op], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
    }

    switch (op) {
        case op_sum:
        case op_subtraction:
            return parser_operation_additive(lh, rh, op);
        case op_multi:
            return parser_operation_multiply(lh, rh);
        case op_pow:
            return parser_operation_power(lh, rh);
        case op_mod:
            if (lh->type != Int) parser_raise_error(p, invalid_operation_mod, NULL, TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
            if (rh->type != Int) parser_raise_error(p, invalid_operation_mod, NULL, TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
        case op_div:
            return parser_operation_divide(lh, rh, op);
        case op_and:
            return parser_operation_and(lh, rh);
        case op_or:
            return parser_operation_or(lh, rh);
        default:
            return lh;
    }
}
