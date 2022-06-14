#include "include/parser.h"
#include "include/parser_operations.h"
#include "include/parser_errors.h"
#include "include/parser_utils.h"
#include "include/lexer.h"
#include "include/variable.h"
#include "include/scope.h"
#include "include/token.h"
#include "include/literal.h"
#include "include/str_to_int.h"
#include "include/str_to_float.h"
#include "include/type.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

parser* init_parser(char* src) {
	parser* p = calloc(1, sizeof(parser));
    p->lexer = lexer_init(src);
    p->global_scope = scope_init();
    p->current_token = NULL;
    p->prev_token = NULL;
    return p;
}

int parser_parse(parser* p) {
	parser_block(p);

    scope_scan(p->global_scope);

	return 0;
}

void parser_program(parser* p) {
    // program ::= function

    parser_get_token(p);
    while (p->current_token->type != token_EOF) parser_function(p);
}

void parser_function(parser* p) {
    // function ::= signature block

    parser_signature(p);
    parser_block(p);
}

void parser_signature(parser* p) {
    // signature ::= identifier '(' argument* ')' ('->' type)?

    parser_identifier(p);

    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_gp_op);

    parser_get_token(p);
    while (p->current_token->type != token_gp_cp) {
        parser_argument(p);
        parser_get_token(p);
    }

    parser_assert_token_type(p, p->current_token->type, token_gp_cp);

    //TODO: implement return annotation
}

void parser_argument(parser* p) {
    // argument ::= identifier type_annotation

    parser_identifier(p);
    parser_type_annotation(p);
}

type parser_type_annotation(parser* p) {
    // type_annotation ::= ':' type

    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_op_colon);

    return parser_type(p);
}

void parser_simple_statement(parser* p, int semicolon_override) {
    // statement ::= (declaration|assignment)

    switch (p->current_token->type) {
        case token_VAR:
        case token_LET:
            parser_declaration(p, NULL);
            break;
        case token_RETURN:
            parser_return(p);
            break;
        default:
            parser_assignment(p, NULL);
    }

    if (semicolon_override) return;
    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_gp_semicolon);
}

void parser_block(parser* p) {
    // block ::=  '{' (statement|control_sequence|return)+ '}'

    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_gp_ocb);
    token* t_end = parser_find_block_end(p, 1);

    parser_get_token(p);

    while (!token_compare(p->current_token, t_end)) {
        parser_statement(p, 0);
        parser_get_token(p);
    }
}

void parser_control_sequence(parser* p, token_type t_type) {
    // control_sequence ::= (std_while|do_while|for|if)

    switch (t_type) {
        case token_IF:
            parser_if(p);
            break;
        case token_FOR:
            parser_for(p);
            break;
        case token_WHILE:
            parser_std_while(p);
            break;
        case token_DO:
            parser_do_while(p);
            break;
        default:
            exit(1);
    }
}

void parser_statement(parser* p, int semicolon_override) {
    switch (p->current_token->type) {
        case token_IF:
        case token_FOR:
        case token_WHILE:
        case token_DO:
            parser_control_sequence(p, p->current_token->type);
            break;
        default:
            parser_simple_statement(p, semicolon_override);
    }
}

void parser_std_while(parser* p) {
    // std_while ::= 'while' condition block

    parser_assert_token_type(p, p->current_token->type, token_WHILE);

    parser condition_parser = parser_make_copy(p);

    while (1) {
        if (!parser_condition(p)) break;

        if (lexer_peek_token(p->lexer)->type == token_gp_ocb) parser_block(p);
        else {
            parser_get_token(p);
            parser_simple_statement(p, 0);
        }

        parser_return_to(p, condition_parser);
    }
    parser_skip_block_or_statement(p);
}

void parser_do_while(parser* p) {
    // do_while ::= 'do' block 'while' condition

    parser_assert_token_type(p, p->current_token->type, token_DO);

    parser block_parser = parser_make_copy(p);

    while (1) {
        if (lexer_peek_token(p->lexer)->type == token_gp_ocb) parser_block(p);
        else {
            parser_get_token(p);
            parser_simple_statement(p, 0);
        }

        parser_get_token(p);
        parser_assert_token_type(p, p->current_token->type, token_WHILE);

        if (!parser_condition(p)) break;

        parser_return_to(p, block_parser);
    }

    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_gp_semicolon);
}

void parser_for(parser* p) {
    // for ::= 'for' '(' statement? ';' relational_expression ';' simple_expression ')' block

    parser condition_parser;
    parser block_parser;
    parser statement_parser;

    parser_assert_token_type(p, p->current_token->type, token_FOR);

    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_gp_op);

    // STATEMENT

    if (lexer_peek_token(p->lexer)->type != token_gp_semicolon) {
        parser_get_token(p);
        parser_simple_statement(p, 0);
    }
    
    // parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_gp_semicolon);

    // CONDITION

    condition_parser = parser_make_copy(p);

    parser_get_token(p);
    parser_skip_to(p, token_gp_semicolon);

    parser_assert_token_type(p, p->current_token->type, token_gp_semicolon);

    // EXPRESSION

    statement_parser = parser_make_copy(p);

    parser_skip_to(p, token_gp_cp);

    parser_assert_token_type(p, p->current_token->type, token_gp_cp);

    block_parser = parser_make_copy(p);

    while (1) {
        parser_return_to(p, condition_parser);
        
        if (!*((int *)(parser_expression(p))->value)) break;

        parser_return_to(p, block_parser);

        if (lexer_peek_token(p->lexer)->type == token_gp_ocb) parser_block(p);
        else {
            parser_get_token(p);
            parser_statement(p, 0);
        }

        parser_return_to(p, statement_parser);
        
        parser_get_token(p);
        
        parser_statement(p, 1);
    }
    parser_return_to(p, block_parser);

    parser_skip_block_or_statement(p);
    
    // parser_get_token(p);
    // parser_assert_token_type(p, p->current_token->type, token_gp_semicolon);
}

boolean parser_condition(parser* p) {
    // condition ::= '(' expression ')'

    boolean condition_result;
    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_gp_op);

    condition_result = *((boolean*)(parser_expression(p))->value);
    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_gp_cp);

    return condition_result;
}

void parser_if(parser* p) {
    // if ::= 'if' condition (block|statement) else?

    boolean result;

    parser_assert_token_type(p, p->current_token->type, token_IF);

    result = parser_condition(p);
    if (result) {
        if (lexer_peek_token(p->lexer)->type == token_gp_ocb) parser_block(p);
        else { 
            parser_get_token(p);
            parser_simple_statement(p, 0);
        }
    }
    else parser_skip_block_or_statement(p);
    
    if (lexer_peek_token(p->lexer)->type == token_ELSE) parser_else(p, result);
}

void parser_else(parser* p, boolean result_if) {
    // else ::= 'else' block

    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_ELSE);

    if (!result_if) {
        if (lexer_peek_token(p->lexer)->type == token_gp_ocb) parser_block(p);
        else {
            parser_get_token(p);
            parser_simple_statement(p, 0);
        }
    }
    else parser_skip_block_or_statement(p);    
}



void parser_return(parser* p) {
    // return ::= 'return' expression
}



variable parser_declaration(parser* p, scope* s) {
    // declaration ::= declaration_directive identifier type_annotation assignment_expression? 

    variable *var = variable_init();
    var->is_constant = parser_declaration_directive(p);
    var->identifier = parser_identifier(p);
    var->type = parser_type_annotation(p);
    var->value = NULL;
    if (scope_get_variable_by_id((s ? s : p->global_scope), var->identifier)) {
        parser_raise_error(p, variable_redeclaration, NULL, var->identifier);
    }
    if (lexer_peek_token(p->lexer)->type == token_op_assignment) var->value = parser_assignment_expression(p, var->type)->value;
    scope_set_variable((s ? s : p->global_scope), *var);
    
    return *var;
}

int parser_declaration_directive(parser* p) {
    // declaration_directive ::= ('let'|'var')

    switch (p->current_token->type) {
        case token_LET: return 1;
        case token_VAR: return 0;
        default: parser_raise_error(p, unexpected_token, NULL, TOKEN_STRING[p->current_token->type], TOKEN_STRING[token_gp_op]);
    }
    return 0;
}

void parser_assignment(parser* p, scope* s) {
    // assignment ::= identifier assignment_expression

    parser_assert_token_type(p, p->current_token->type, token_identifier);
    variable* var = scope_get_variable_by_id((s ? s : p->global_scope), p->current_token->value);
    if (var->is_constant) parser_raise_error(p, illegal_assignment_constant, NULL, var->identifier);
    var->value = (parser_assignment_expression(p, var->type))->value;
    scope_set_variable((s ? s : p->global_scope), *var);
}

literal* parser_assignment_expression(parser* p, type expected_type) {
    // assignment_expression ::= '=' expression

    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_op_assignment);

    literal* lit = parser_expression(p);

    if (!type_compare(lit->type, expected_type)) {
        if (type_is_number(lit->type) && type_is_number(expected_type)) {
            if (type_compare(lit->type, Int))
                lit = literal_cast_to_float(lit);
            else
                lit = literal_cast_to_float(lit);
        }
        else parser_raise_error(p, type_mismatch, NULL, TYPE_STRING[lit->type], TYPE_STRING[expected_type]);
    }

    return lit;
}

literal* parser_expression(parser* p) {
    // expression ::= simple_expression (relational_operator simple_expression)?

    literal* lh;
    literal* rh;
    int* result = (int*) malloc(sizeof(int));
    lh = parser_simple_expression(p);
    token_type op_token_type;

    if (token_is_relational_operator(lexer_peek_token(p->lexer))) {
        parser_get_token(p);
        op_token_type = p->current_token->type;
        rh = parser_simple_expression(p);

        switch (op_token_type) {
            case token_op_lt:
                *result = parser_operation_comparation(p, lh, rh, op_lt);
                break;
            case token_op_lte:
                *result = parser_operation_comparation(p, lh, rh, op_lte);
                break;
            case token_op_gt:
                *result = parser_operation_comparation(p, lh, rh, op_gt);
                break;
            case token_op_gte:
                *result = parser_operation_comparation(p, lh, rh, op_gte);
                break;
            case token_op_eq:
                *result = parser_operation_equality(p, lh, rh, op_eq);
                break;
            case token_op_neq:
                *result = parser_operation_equality(p, lh, rh, op_neq);
                break;
            default: break;
        }
        lh->type = Bool;
        lh->value = result;
    }

    return lh;
}

literal* parser_simple_expression(parser* p) {
    // simple_expression ::= unary_operator? term (lesser_precedence_operator term)*

    token_type op_token_type;

    if (token_is_unary_operator(lexer_peek_token(p->lexer))) {
        parser_get_token(p);
        op_token_type = p->current_token->type;
    }

    literal* lh = parser_term(p);
    
    switch (op_token_type) {
        case token_op_plus:
            if (!type_is_number(lh->type)) parser_raise_error(p, invalid_operation_unary, NULL, OP_STRING[op_sum], TYPE_STRING[lh->type]);
            break;
        case token_op_minus:                    
            if (!type_is_number(lh->type))
                parser_raise_error(p, invalid_operation_unary, NULL, OP_STRING[op_subtraction], TYPE_STRING[lh->type]);
            lh = parser_operation_multiply(lh, literal_init_with_int(-1));
            break;
        case token_op_NOT:
            if (!type_compare(lh->type, Bool))
                parser_raise_error(p, invalid_operation_unary, NULL, OP_STRING[op_not], TYPE_STRING[lh->type]);
            lh = parser_operation_not(lh);
            break;
        default: break;
    }

    literal* rh;
    literal* result;
    
    while(token_is_lesser_precedence_operator(lexer_peek_token(p->lexer))) {
        parser_get_token(p);
        op_token_type = p->current_token->type;
        rh = parser_term(p);

        switch (op_token_type) {
            case token_op_plus:
                result = parser_operation(p, op_sum, lh, rh);
                break;
            case token_op_minus:
                result = parser_operation(p, op_subtraction, lh, rh);
                break;
            case token_op_OR:
                if (!type_compare(lh->type, Bool) || !type_compare(rh->type, Bool))
                    parser_raise_error(p, invalid_operation, NULL, OP_STRING[op_or], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
                result = parser_operation(p, op_or, lh, rh);
                break;
            default: break;
        }
        *lh = *result;
    }
        
    return lh;
}

literal* parser_term(parser* p) {
    // term ::= factor (higher_precedence_operator factor)*

    literal* lh = parser_factor(p);
    literal* rh;
    literal* result;
    token_type op_t_type;
    
    while(token_is_higher_precedence_operator(lexer_peek_token(p->lexer))) {
        parser_get_token(p);
        op_t_type = p->current_token->type;
        rh = parser_factor(p);
        switch (op_t_type) {
            case token_op_div:
                result = parser_operation(p, op_div, lh, rh);
                break;
            case token_op_mod:
                result = parser_operation(p, op_mod, lh, rh);
                break;
            case token_op_multi:
                result = parser_operation(p, op_multi, lh, rh);
                break;
            case token_op_pow:
                if (rh->type != Int) parser_raise_error(p, invalid_operation_pow, NULL);
                result = parser_operation(p, op_pow, lh, literal_init_with_int(*(int*)rh->value));
                break;
            case token_op_AND:
                if (!type_compare(lh->type, Bool) || !type_compare(rh->type, Bool))
                    parser_raise_error(p, invalid_operation, NULL, OP_STRING[op_and], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
                result = parser_operation(p, op_and, lh, rh);
                break;
            default: break;
        }
        *lh = *result;
    }
        
    return lh;
}

literal* parser_factor(parser* p) {
    // factor ::= identifier | string | number | bool | grouped_expression | !factor

    parser_get_token(p);
    int is_negative = 0;

    switch (p->current_token->type) {
        case token_identifier:
            variable* var = hashmap_get(p->global_scope->var_space, &(variable){identifier:p->current_token->value});
            if (!var) parser_raise_error(p, undefined_identifier, NULL, p->current_token->value);
            return literal_init_from_var(var);
        case token_literal_string:
            return literal_init_with_string(p->current_token->value);
        case token_op_minus:
            is_negative = 1;
        case token_op_plus:
            parser_get_token(p);
        case token_literal_int:
            return parser_number(p, 1, is_negative);
        case token_literal_float:
            return parser_number(p, 0, is_negative);
        case token_literal_true:   
            return literal_init_with_bool(True);
        case token_literal_false:
            return literal_init_with_bool(False);
        case token_gp_op:
            return parser_grouped_expression(p);
        case token_op_NOT:
            return parser_operation_not(parser_factor(p));
        default:
            parser_raise_error(p, unexpected_token, NULL, TOKEN_STRING[p->current_token->type], "token_identifier|token_literal_string|token_op_minus|token_op_plus|token_literal_int|token_literal_float|token_gp_op");
            return NULL;
    }
}

literal* parser_grouped_expression(parser* p) {
    // grouped_expression ::= '(' expression ')'

    literal* lh;
    lh = parser_expression(p);
    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_gp_cp);

    return lh;
}

literal* parser_number(parser* p, int is_int, int is_negative) {
    // number ::= sign? (integer|float)

    if (is_int) return parser_integer(p, is_negative);
    return parser_float(p, is_negative);
}

literal* parser_integer(parser* p, int is_negative) {
    // integer ::= [0-9]+
    
    int* i = (int*) malloc(sizeof(int));
    str2num_errno a = {.discriminant = DU_INT, .i_errno = str2int(i, p->current_token->value, 10)};
    if (a.i_errno == STR2INT_SCIENTIFICNOTATION) {
        *i = *(float*)parser_float(p, is_negative)->value;
    }
    else {
        parser_assert_number_conversion(p, a);
    }
    
    if (!is_negative) return literal_init_with_int(*i);
    return parser_operation_multiply(literal_init_with_int(*i), literal_init_with_int(-1));
}

literal* parser_float(parser* p, int is_negative) {
    // float ::= [0-9]* '.' [0-9]* ('e' (lesser_precedence_binary_arithmetic_operator)? [0-9]+)?

    float* f = (float*) malloc(sizeof(float));
    str2num_errno a = {.discriminant = DU_FLOAT, .f_errno = str2float(f, p->current_token->value)};
    parser_assert_number_conversion(p, a);

    if (!is_negative) return literal_init_with_float(*f);
    return parser_operation_multiply(literal_init_with_float(*f), literal_init_with_int(-1));
}

char* parser_identifier(parser* p) {
    // identifier ::= ([a-z]|[A-Z]) ([a-z]|[A-Z]|[0-9])*

    parser_get_token(p);
    parser_assert_token_type(p, p->current_token->type, token_identifier);

    return p->current_token->value;
}

type parser_type(parser* p) {
    // type ::= ('Int'|'Float'|'Bool'|'String')
    parser_get_token(p);
    switch (p->current_token->type) {
        case token_type_INT: return Int;
        case token_type_FLOAT: return Float;
        case token_type_BOOL: return Bool;
        case token_type_STRING: return String;
        default: return Null;
    }
}
