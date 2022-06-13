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

Parser* init_parser(char* src) {
	Parser* parser = calloc(1, sizeof(Parser));
    parser->lexer = lexer_init(src);
    parser->global_scope = scope_init();
    parser->current_token = NULL;
    parser->prev_token = NULL;
    return parser;
}

int parser_parse(Parser* parser) {
	parser_block(parser);

    scope_scan(parser->global_scope);

	return 0;
}

void parser_program(Parser* parser) {
    // program ::= function

    parser_get_token(parser);
    while (parser->current_token->type != token_EOF) parser_function(parser);
}

void parser_function(Parser* parser) {
    // function ::= signature block

    parser_signature(parser);
    parser_block(parser);
}

void parser_signature(Parser* parser) {
    // signature ::= identifier '(' argument* ')' ('->' type)?

    parser_identifier(parser);

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_op);

    parser_get_token(parser);
    while (parser->current_token->type != token_gp_cp) {
        parser_argument(parser);
        parser_get_token(parser);
    }

    parser_assert_token_type(parser, parser->current_token->type, token_gp_cp);

    //TODO: implement return annotation
}

void parser_argument(Parser* parser) {
    // argument ::= identifier type_annotation

    parser_identifier(parser);
    parser_type_annotation(parser);
}

Type parser_type_annotation(Parser* parser) {
    // type_annotation ::= ':' type

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_op_colon);

    return parser_type(parser);
}

void parser_simple_statement(Parser* parser, int semicolon_override) {
    // statement ::= (declaration|assignment)

    switch (parser->current_token->type) {
        case token_VAR:
        case token_LET:
            parser_declaration(parser, NULL);
            break;
        case token_RETURN:
            parser_return(parser);
            break;
        default:
            parser_assignment(parser, NULL);
    }

    if (semicolon_override) return;
    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_semicolon);
}

void parser_block(Parser* parser) {
    // block ::=  '{' (statement|control_sequence|return)+ '}'

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_ocb);
    Token* end = parser_find_block_end(parser, 1);

    parser_get_token(parser);

    while (!token_compare(parser->current_token, end)) {
        parser_statement(parser, 0);
        parser_get_token(parser);
    }
}

void parser_control_sequence(Parser* parser, TokenType tokenType) {
    // control_sequence ::= (std_while|do_while|for|if)

    switch (tokenType) {
        case token_IF:
            parser_if(parser);
            break;
        case token_FOR:
            parser_for(parser);
            break;
        case token_WHILE:
            parser_std_while(parser);
            break;
        case token_DO:
            parser_do_while(parser);
            break;
        default:
            exit(1);
    }
}

void parser_statement(Parser* parser, int semicolon_override) {
    switch (parser->current_token->type) {
        case token_IF:
        case token_FOR:
        case token_WHILE:
        case token_DO:
            parser_control_sequence(parser, parser->current_token->type);
            break;
        default:
            parser_simple_statement(parser, semicolon_override);
    }
}

void parser_std_while(Parser* parser) {
    // std_while ::= 'while' condition block

    parser_assert_token_type(parser, parser->current_token->type, token_WHILE);

    Parser condition_parser = parser_make_copy(parser);

    while (1) {
        if (!parser_condition(parser)) break;

        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else {
            parser_get_token(parser);
            parser_simple_statement(parser, 0);
        }

        parser_return_to(parser, condition_parser);
    }
    parser_skip_block_or_statement(parser);
}

void parser_do_while(Parser* parser) {
    // do_while ::= 'do' block 'while' condition

    parser_assert_token_type(parser, parser->current_token->type, token_DO);

    Parser block_parser = parser_make_copy(parser);

    while (1) {
        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else {
            parser_get_token(parser);
            parser_simple_statement(parser, 0);
        }

        parser_get_token(parser);
        parser_assert_token_type(parser, parser->current_token->type, token_WHILE);

        if (!parser_condition(parser)) break;

        parser_return_to(parser, block_parser);
    }

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_semicolon);
}

void parser_for(Parser* parser) {
    // for ::= 'for' '(' statement? ';' relational_expression ';' simple_expression ')' block

    Parser condition_parser;
    Parser block_parser;
    Parser statement_parser;

    parser_assert_token_type(parser, parser->current_token->type, token_FOR);

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_op);

    // STATEMENT

    if (lexer_peek_token(parser->lexer)->type != token_gp_semicolon) {
        parser_get_token(parser);
        parser_simple_statement(parser, 0);
    }
    
    // parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_semicolon);

    // CONDITION

    condition_parser = parser_make_copy(parser);

    parser_get_token(parser);
    parser_skip_to(parser, token_gp_semicolon);

    parser_assert_token_type(parser, parser->current_token->type, token_gp_semicolon);

    // EXPRESSION

    statement_parser = parser_make_copy(parser);

    parser_skip_to(parser, token_gp_cp);

    parser_assert_token_type(parser, parser->current_token->type, token_gp_cp);

    block_parser = parser_make_copy(parser);

    while (1) {
        parser_return_to(parser, condition_parser);
        
        if (!*((int *)(parser_expression(parser))->value)) break;

        parser_return_to(parser, block_parser);

        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else {
            parser_get_token(parser);
            parser_statement(parser, 0);
        }

        parser_return_to(parser, statement_parser);
        
        parser_get_token(parser);
        
        parser_statement(parser, 1);
    }
    parser_return_to(parser, block_parser);

    parser_skip_block_or_statement(parser);
    
    // parser_get_token(parser);
    // parser_assert_token_type(parser, parser->current_token->type, token_gp_semicolon);
}

Boolean parser_condition(Parser* parser) {
    // condition ::= '(' expression ')'

    Boolean condition_result;
    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_op);

    condition_result = *((Boolean*)(parser_expression(parser))->value);
    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_cp);

    return condition_result;
}

void parser_if(Parser* parser) {
    // if ::= 'if' condition (block|statement) else?

    Boolean result;

    parser_assert_token_type(parser, parser->current_token->type, token_IF);

    result = parser_condition(parser);
    if (result) {
        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else { 
            parser_get_token(parser);
            parser_simple_statement(parser, 0);
        }
    }
    else parser_skip_block_or_statement(parser);
    
    if (lexer_peek_token(parser->lexer)->type == token_ELSE) parser_else(parser, result);
}

void parser_else(Parser* parser, Boolean result_if) {
    // else ::= 'else' block

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_ELSE);

    if (!result_if) {
        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else {
            parser_get_token(parser);
            parser_simple_statement(parser, 0);
        }
    }
    else parser_skip_block_or_statement(parser);    
}



void parser_return(Parser* parser) {
    // return ::= 'return' expression
}



Variable parser_declaration(Parser* parser, Scope* scope) {
    // declaration ::= declaration_directive identifier type_annotation assignment_expression? 

    Variable *var = variable_init();
    var->isConstant = parser_declaration_directive(parser);
    var->identifier = parser_identifier(parser);
    var->type = parser_type_annotation(parser);
    var->value = NULL;
    if (scope_get_variable_by_id((scope ? scope : parser->global_scope), var->identifier)) {
        parser_raise_error(parser, variable_redeclaration, NULL, var->identifier);
    }
    if (lexer_peek_token(parser->lexer)->type == token_op_assignment) var->value = parser_assignment_expression(parser, var->type)->value;
    scope_set_variable((scope ? scope : parser->global_scope), *var);
    
    return *var;
}

int parser_declaration_directive(Parser* parser) {
    // declaration_directive ::= ('let'|'var')

    switch (parser->current_token->type) {
        case token_LET: return 1;
        case token_VAR: return 0;
        default: parser_raise_error(parser, unexpected_token, NULL, TOKEN_STRING[parser->current_token->type], TOKEN_STRING[token_gp_op]);
    }
    return 0;
}

void parser_assignment(Parser* parser, Scope* scope) {
    // assignment ::= identifier assignment_expression

    parser_assert_token_type(parser, parser->current_token->type, token_identifier);
    Variable* var = scope_get_variable_by_id((scope ? scope : parser->global_scope), parser->current_token->value);
    if (var->isConstant) parser_raise_error(parser, illegal_assignment_constant, NULL, var->identifier);
    var->value = (parser_assignment_expression(parser, var->type))->value;
    scope_set_variable((scope ? scope : parser->global_scope), *var);
}

Literal* parser_assignment_expression(Parser* parser, Type expected_type) {
    // assignment_expression ::= '=' expression

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_op_assignment);

    Literal* lit = parser_expression(parser);

    parser_assert_type(parser, lit->type, expected_type);

    return lit;
}

Literal* parser_expression(Parser* parser) {
    // expression ::= simple_expression (relational_operator simple_expression)?

    Literal* lh;
    Literal* rh;
    int* result = (int*) malloc(sizeof(int));
    lh = parser_simple_expression(parser);
    TokenType op_token_type;

    if (token_is_relational_operator(lexer_peek_token(parser->lexer))) {
        parser_get_token(parser);
        op_token_type = parser->current_token->type;
        rh = parser_simple_expression(parser);

        switch (op_token_type) {
            case token_op_lt:
                *result = parser_operation_comparation(parser, lh, rh, op_lt);
                break;
            case token_op_lte:
                *result = parser_operation_comparation(parser, lh, rh, op_lte);
                break;
            case token_op_gt:
                *result = parser_operation_comparation(parser, lh, rh, op_gt);
                break;
            case token_op_gte:
                *result = parser_operation_comparation(parser, lh, rh, op_gte);
                break;
            case token_op_eq:
                *result = parser_operation_equality(parser, lh, rh, op_eq);
                break;
            case token_op_neq:
                *result = parser_operation_equality(parser, lh, rh, op_neq);
                break;
            default: break;
        }
        lh->type = Bool;
        lh->value = result;
    }

    return lh;
}

Literal* parser_simple_expression(Parser* parser) {
    // simple_expression ::= unary_operator? term (lesser_precedence_operator term)*

    TokenType op_token_type;

    if (token_is_unary_operator(lexer_peek_token(parser->lexer))) {
        parser_get_token(parser);
        op_token_type = parser->current_token->type;
    }

    Literal* lh = parser_term(parser);
    
    switch (op_token_type) {
        case token_op_plus:
            if (!type_is_number(lh->type)) parser_raise_error(parser, invalid_operation_unary, NULL, OP_STRING[op_sum], TYPE_STRING[lh->type]);
            break;
        case token_op_minus:                    
            if (!type_is_number(lh->type))
                parser_raise_error(parser, invalid_operation_unary, NULL, OP_STRING[op_subtraction], TYPE_STRING[lh->type]);
            lh = parser_operation_multiply(lh, literal_init_with_int(-1));
            break;
        case token_op_NOT:
            if (!type_compare(lh->type, Bool))
                parser_raise_error(parser, invalid_operation_unary, NULL, OP_STRING[op_not], TYPE_STRING[lh->type]);
            lh = parser_operation_not(lh);
            break;
        default: break;
    }

    Literal* rh;
    Literal* result;
    
    while(token_is_lesser_precedence_operator(lexer_peek_token(parser->lexer))) {
        parser_get_token(parser);
        op_token_type = parser->current_token->type;
        rh = parser_term(parser);

        switch (op_token_type) {
            case token_op_plus:
                result = parser_operation(parser, op_sum, lh, rh);
                break;
            case token_op_minus:
                result = parser_operation(parser, op_subtraction, lh, rh);
                break;
            case token_op_OR:
                if (!type_compare(lh->type, Bool) || !type_compare(rh->type, Bool))
                    parser_raise_error(parser, invalid_operation, NULL, OP_STRING[op_or], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
                result = parser_operation(parser, op_or, lh, rh);
                break;
            default: break;
        }
        *lh = *result;
    }
        
    return lh;
}

Literal* parser_term(Parser* parser) {
    // term ::= factor (higher_precedence_operator factor)*

    Literal* lh = parser_factor(parser);
    Literal* rh;
    Literal* result;
    TokenType op_token_type;
    
    while(token_is_higher_precedence_operator(lexer_peek_token(parser->lexer))) {
        parser_get_token(parser);
        op_token_type = parser->current_token->type;
        rh = parser_factor(parser);
        switch (op_token_type) {
            case token_op_div:
                result = parser_operation(parser, op_div, lh, rh);
                break;
            case token_op_mod:
                result = parser_operation(parser, op_mod, lh, rh);
                break;
            case token_op_multi:
                result = parser_operation(parser, op_multi, lh, rh);
                break;
            case token_op_pow:
                if (rh->type != Int) parser_raise_error(parser, invalid_operation_pow, NULL);
                result = parser_operation(parser, op_pow, lh, literal_init_with_int(*(int*)rh->value));
                break;
            case token_op_AND:
                if (!type_compare(lh->type, Bool) || !type_compare(rh->type, Bool))
                    parser_raise_error(parser, invalid_operation, NULL, OP_STRING[op_and], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
                result = parser_operation(parser, op_and, lh, rh);
                break;
            default: break;
        }
        *lh = *result;
    }
        
    return lh;
}

Literal* parser_factor(Parser* parser) {
    // factor ::= identifier | string | number | bool | grouped_expression | !factor

    parser_get_token(parser);
    int is_negative = 0;

    switch (parser->current_token->type) {
        case token_identifier:
            Variable* var = hashmap_get(parser->global_scope->var_space, &(Variable){identifier:parser->current_token->value});
            if (!var) parser_raise_error(parser, undefined_identifier, NULL, parser->current_token->value);
            return literal_init_from_var(var);
        case token_literal_string:
            return literal_init_with_string(parser->current_token->value);
        case token_op_minus:
            is_negative = 1;
        case token_op_plus:
            parser_get_token(parser);
        case token_literal_int:
            return parser_number(parser, 1, is_negative);
        case token_literal_float:
            return parser_number(parser, 0, is_negative);
        case token_literal_true:   
            return literal_init_with_bool(True);
        case token_literal_false:
            return literal_init_with_bool(False);
        case token_gp_op:
            return parser_grouped_expression(parser);
        case token_op_NOT:
            return parser_operation_not(parser_factor(parser));
        default:
            parser_raise_error(parser, unexpected_token, NULL, TOKEN_STRING[parser->current_token->type], "token_identifier|token_literal_string|token_op_minus|token_op_plus|token_literal_int|token_literal_float|token_gp_op");
            return NULL;
    }
}

Literal* parser_grouped_expression(Parser* parser) {
    // grouped_expression ::= '(' expression ')'

    Literal* lh;
    lh = parser_expression(parser);
    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_cp);
    return lh;
}

Literal* parser_number(Parser* parser, int is_int, int is_negative) {
    // number ::= sign? (integer|float)

    if (is_int) return parser_integer(parser, is_negative);
    return parser_float(parser, is_negative);
}

Literal* parser_integer(Parser* parser, int is_negative) {
    // integer ::= [0-9]+
    
    int* i = (int*) malloc(sizeof(int));
    str2num_errno a = {.discriminant = DU_INT, .i_errno = str2int(i, parser->current_token->value, 10)};
    if (a.i_errno == STR2INT_SCIENTIFICNOTATION) {
        *i = *(float*)parser_float(parser, is_negative)->value;
    }
    else {
        parser_assert_number_conversion(parser, a);
    }
    
    if (!is_negative) return literal_init_with_int(*i);
    return parser_operation_multiply(literal_init_with_int(*i), literal_init_with_int(-1));
}

Literal* parser_float(Parser* parser, int is_negative) {
    // float ::= [0-9]* '.' [0-9]* ('e' (lesser_precedence_binary_arithmetic_operator)? [0-9]+)?

    float* f = (float*) malloc(sizeof(float));
    str2num_errno a = {.discriminant = DU_FLOAT, .f_errno = str2float(f, parser->current_token->value)};
    parser_assert_number_conversion(parser, a);

    if (!is_negative) return literal_init_with_float(*f);
    return parser_operation_multiply(literal_init_with_float(*f), literal_init_with_int(-1));
}

char* parser_identifier(Parser* parser) {
    // identifier ::= ([a-z]|[A-Z]) ([a-z]|[A-Z]|[0-9])*
    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_identifier);

    return parser->current_token->value;
}

Type parser_type(Parser* parser) {
    // type ::= ('Int'|'Float'|'Bool'|'String')
    parser_get_token(parser);
    switch (parser->current_token->type) {
        case token_type_INT: return Int;
        case token_type_FLOAT: return Float;
        case token_type_BOOL: return Bool;
        case token_type_STRING: return String;
        default: return Null;
    }
}
