#include "include/parser.h"
#include "include/lexer.h"
#include "include/variable.h"
#include "include/scope.h"
#include "include/token.h"
#include "include/literal.h"
#include "include/str_to_int.h"
#include "include/str_to_float.h"
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

void parser_destroy_all(Parser* parser) {
    hashmap_free(parser->global_scope->var_space);
    free(parser->global_scope);
    free(parser->lexer);
    free(parser->current_token);
    free(parser->prev_token);
    free(parser);
    fprintf(stderr, "[Parser.c] Freed used resources.\n");
}

void parser_raise_error(Parser* parser, parse_error e, const char* err_str_arg, ...) {
    va_list args;
    va_start(args, err_str_arg);
    char* format = "[Parser.c:%d:%d]: %s\n";

    // format = (char*) realloc(format, strlen(format) + strlen(err_str_arg) + 8);

    // const char* template = "<type={%s}, int_type={%d}, value={%s}>";
    char* err_str;
    switch (e) {
        case illegal_assignment_constant:
            err_str = ERR_ILLEGAL_ASSIGNMENT_CONSTANT;
            break;
        case invalid_int_literal:
            err_str = ERR_INVALID_INT_LITERAL;
            break;
        case invalid_float_literal:
            err_str = ERR_INVALID_FLOAT_LITERAL;
            break;
        case invalid_operation:
            err_str = ERR_INVALID_OPERATION;
            break;
        case invalid_operation_mod:
            err_str = ERR_INVALID_OPERATION_MOD;
            break;
        case invalid_operation_pow:
            err_str = ERR_INVALID_OPERATION_POW;
            break;
        case invalid_operation_unary:
            err_str = ERR_INVALID_OPERATION_UNARY;
            break;
        case type_mismatch:
            err_str = ERR_TYPE_MISMATCH;
            break;
        case undefined_error:
            err_str = ERR_UNDEFINED_ERROR;
            err_str = malloc((strlen(err_str_arg) + 1) * sizeof(char));
            strcpy(err_str, err_str_arg);
            break;
        case undefined_identifier:
            err_str = ERR_UNDEFINED_IDENTIFIER;
            break;
        case unexpected_eof:
            err_str = ERR_UNEXPECTED_EOF;
            break;
        case unexpected_token:
            err_str = ERR_UNEXPECTED_TOKEN;
            break;
        case variable_redeclaration:
            err_str = ERR_VARIABLE_REDECLARATION;
            break;
        
    }
    // char* str = malloc((strlen(format) + strlen(err_str_arg) + 8) * sizeof(char));

    char* str = malloc((strlen(format) + strlen(err_str) + 8) * sizeof(char));
    sprintf(str, format, parser->lexer->t_pos->line, parser->lexer->t_pos->col, err_str);

    vfprintf(stderr, str, args);

    va_end(args);
    free(str);
    if (e == undefined_error) free(err_str);
    parser_destroy_all(parser);
    exit(1);
}

void parser_assert_variable_type(Parser* parser, Type t1, Type t2) {
    if (variable_compare_type(t1, t2)) return;
    parser_raise_error(parser, type_mismatch, NULL, TYPE_STRING[t1], TYPE_STRING[t2]);
}

void parser_assert_token_type(Parser* parser, TokenType t1, TokenType t2) {
    if (t1 == t2) return;
    parser_raise_error(parser, unexpected_token, NULL, TOKEN_STRING[t1], TOKEN_STRING[t2]);
}

void parser_assert_number_conversion(Parser* parser, str2num_errno res_code) {
    if (res_code.discriminant == DU_INT && res_code.i_errno == STR2INT_SUCCESS) return;
    if (res_code.discriminant == DU_FLOAT && res_code.f_errno == STR2FLOAT_SUCCESS) return;

    parser_raise_error(parser, (res_code.discriminant == DU_INT ? invalid_int_literal : invalid_float_literal), NULL, parser->current_token->value);
}

void parser_get_token(Parser* parser) {
    parser->prev_token = parser->current_token;
    parser->current_token = lexer_read_token(parser->lexer);
}

void parser_jump(Parser* parser, int i) {
    parser->lexer->i = i;
    parser->lexer->c = parser->lexer->src[parser->lexer->i];
}

void parser_skip_block_or_statement(Parser* parser) {
    int is_block = 0;
    switch(lexer_peek_token(parser->lexer)->type) {
        case token_gp_ocb:
            is_block = 1;
            break;
        case token_EOF:
            parser_raise_error(parser, unexpected_eof, NULL);
        default: break;
    }
    // if (parser->current_token)
    while(parser->current_token->type != (is_block ? token_gp_ccb : token_gp_semicolon) && parser->current_token->type != token_EOF) {
        parser_get_token(parser);
    }
}

void parser_skip_to(Parser* parser, TokenType t) {
    if (lexer_peek_token(parser->lexer)->type == token_EOF)
        parser_raise_error(parser, unexpected_eof, NULL);

    while(parser->current_token->type != t && parser->current_token->type != token_EOF) {
        parser_get_token(parser);
    }
}

int parser_parse(Parser* parser) {
	// Token* token = 0;

	// do {
	// 	token = lexer_read_token(parser->lexer);
	// }
	// while (token->type != token_EOF);
	
	// parser_program(parser);

    // parser_block(parser);
    // printf("%d\n", parser_condition(parser));
    
    // parser_do_while(parser);
    parser_for(parser);

	// printf("%s\n", token_to_str(token));
	// printf("printou todos os simbolos");

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

void parser_statement(Parser* parser, int semicolon_override) {
    // statement ::= (declaration|assignment)

    switch (parser->current_token->type) {
        case token_VAR:
        case token_LET:
            parser_declaration(parser, NULL);
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

    parser_get_token(parser);
    
    while (parser->current_token->type != token_gp_ccb) {
        switch (parser->current_token->type) {
            case token_IF:
            case token_FOR:
            case token_WHILE:
            case token_DO:
                parser_control_sequence(parser, parser->current_token->type);
                break;
            case token_RETURN:
                parser_return(parser);
                break;
            default:
                parser_statement(parser, 0);
        }
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

void parser_std_while(Parser* parser) {
    // std_while ::= 'while' condition block

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_WHILE);

    int condition_pos = parser->lexer->i;

    while (1) {
        if (!parser_condition(parser)) break;

        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else {
            parser_get_token(parser);
            parser_statement(parser, 0);
        }

        parser_jump(parser, condition_pos);
    }

    parser_skip_block_or_statement(parser);
}

void parser_do_while(Parser* parser) {
    // do_while ::= 'do' block 'while' condition

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_DO);

    int block_pos = parser->lexer->i;

    while (1) {
        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else {
            parser_get_token(parser);
            parser_statement(parser, 0);
        }

        parser_get_token(parser);
        parser_assert_token_type(parser, parser->current_token->type, token_WHILE);

        if (!parser_condition(parser)) break;

        parser_jump(parser, block_pos);
    }

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_semicolon);
}

void parser_for(Parser* parser) {
    // for ::= 'for' '(' statement? ';' relational_expression ';' arithmetic_expression ')' block

    int condition_pos = 0;
    int block_pos = 0;
    int statement_pos=0;

    parser_get_token(parser);
    parser_statement(parser, 0);

    parser_get_token(parser);
    parser_statement(parser, 0);

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_FOR);

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_op);

    // STATEMENT

    if (lexer_peek_token(parser->lexer)->type != token_gp_semicolon) {
        parser_get_token(parser);
        parser_statement(parser, 0);
    }

    
    // parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_semicolon);

    // CONDITION

    condition_pos = parser->lexer->i;

    parser_get_token(parser);
    parser_skip_block_or_statement(parser);

    parser_assert_token_type(parser, parser->current_token->type, token_gp_semicolon);

    // EXPRESSION


    statement_pos = parser->lexer->i;

    parser_skip_to(parser, token_gp_cp);

    parser_assert_token_type(parser, parser->current_token->type, token_gp_cp);

    block_pos = parser->lexer->i;

    while (1) {
        parser_jump(parser, condition_pos);
        
        if (!*((int *)(parser_expression(parser))->value)) break;


        parser_jump(parser, block_pos);

        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else {
            parser_get_token(parser);
            parser_statement(parser, 0);
        }

        parser_jump(parser, statement_pos);
        
        parser_get_token(parser);
        
        parser_statement(parser, 1);
        
    }

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_semicolon);
}

int parser_condition(Parser* parser) {
    // condition ::= '(' expression ')'

    int condition_result;
    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_op);

    condition_result = *((int *)(parser_expression(parser))->value);

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_gp_cp);

    return condition_result;
}

void parser_if(Parser* parser) {
    // if ::= 'if' condition (block|statement) else?

    int result;

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_IF);

    result = parser_condition(parser);
    if (result) {
        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else { 
            parser_get_token(parser);
            parser_statement(parser, 0);
        }
    }
    else parser_skip_block_or_statement(parser);


    if (lexer_peek_token(parser->lexer)->type == token_ELSE) parser_else(parser, result);
}

void parser_else(Parser* parser, int result_if) {
    // else ::= 'else' block

    parser_get_token(parser);
    parser_assert_token_type(parser, parser->current_token->type, token_ELSE);

    if (!result_if) {
        if (lexer_peek_token(parser->lexer)->type == token_gp_ocb) parser_block(parser);
        else {
            parser_get_token(parser);
            parser_statement(parser, 0);
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

    parser_assert_variable_type(parser, lit->type, expected_type);

    return lit;
}

int parser_opration_numeric_relational(Parser* parser, Literal* lh, Literal *rh, operation op) { 
    if (!variable_is_number(lh->type)) parser_raise_error(parser, invalid_operation, NULL, OP_STRING[op], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
    if (!variable_is_number(rh->type)) parser_raise_error(parser, invalid_operation, NULL, OP_STRING[op], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);

    switch (op) {
        case op_lt:
            return (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) < (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
        case op_lte:
            return (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) <= (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
        case op_gt:
            return (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) > (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
        case op_gte:
            return (lh->type == Int ? *(int*)lh->value : *(float*)lh->value) >= (rh->type == Int ? *(int*)rh->value : *(float*)rh->value);
        default: return 0;
    }
}

int parser_operation_equality(Parser* parser, Literal* lh, Literal *rh, operation op) {
    // if (!variable_is_number(rh->type)) parser_raise_error(parser, invalid_operation, "Invalid operand for lt comparison");
    return 0;
}

Literal* parser_expression(Parser* parser) {
    // expression ::= arithmetic_expression (relational_operator arithmetic_expression)?

    Literal* lh;
    Literal* rh;
    int* result = (int*) malloc(sizeof(int));
    lh = parser_arithmetic_expression(parser);
    TokenType op_token_type;

    if (token_is_relational_operator(lexer_peek_token(parser->lexer))) {
        parser_get_token(parser);
        op_token_type = parser->current_token->type;
        rh = parser_arithmetic_expression(parser);

        switch (op_token_type) {
            case token_op_lt:
                *result = parser_opration_numeric_relational(parser, lh, rh, op_lt);
                break;
            case token_op_lte:
                *result = parser_opration_numeric_relational(parser, lh, rh, op_lte);
                break;
            case token_op_gt:
                *result = parser_opration_numeric_relational(parser, lh, rh, op_gt);
                break;
            case token_op_gte:
                *result = parser_opration_numeric_relational(parser, lh, rh, op_gte);
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

Literal* parser_operation_additive(Literal* lh, Literal *rh, operation op) {
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

Literal* parser_operation_multiply(Literal* lh, Literal *rh) {
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

Literal* parser_operation_divide(Literal* lh, Literal *rh, operation op) {
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

Literal* parser_operation_mod(Literal* lh, Literal *rh) {
    return lh;
}

Literal* parser_operation_or(Literal* lh, Literal *rh) {
    return lh;
}

Literal* parser_operation_and(Literal* lh, Literal *rh) {
    return lh;
}

Literal* parser_operation_not(Literal* lh) {
    return lh;
}

Literal* parser_operation(Parser* parser, operation op, Literal* lh, Literal *rh) {
    if (op < 6) {
        if (!variable_is_number(lh->type)) parser_raise_error(parser, invalid_operation, NULL, OP_STRING[op], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
        if (!variable_is_number(rh->type)) parser_raise_error(parser, invalid_operation, NULL, OP_STRING[op], TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
    }

    switch (op) {
        case op_sum:
        case op_subtraction:
            return parser_operation_additive(lh, rh, op);
        case op_multi:
            return parser_operation_multiply(lh, rh);
        case op_pow:
            Literal base = *lh;
            for (int i = 1; i < *(int*)rh->value; i++)
                lh = parser_operation_multiply(lh, &base);
            return lh;
        case op_mod:
            if (lh->type != Int) parser_raise_error(parser, invalid_operation_mod, NULL, TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
            if (rh->type != Int) parser_raise_error(parser, invalid_operation_mod, NULL, TYPE_STRING[lh->type], TYPE_STRING[rh->type]);
        case op_div:
            return parser_operation_divide(lh, rh, op);
        case op_and:
        case op_or:
        default:
            return lh;
    }
}

Literal* parser_arithmetic_expression(Parser* parser) {
    // arithmetic_expression ::= unary_operator? term (lesser_precedence_operator term)*

    TokenType op_token_type;

    if (token_is_unary_operator(lexer_peek_token(parser->lexer))) {
        parser_get_token(parser);
        op_token_type = parser->current_token->type;
    }

    Literal* lh = parser_term(parser);
    
    switch (op_token_type) {
        case token_op_plus:
            if (!variable_is_number(lh->type)) parser_raise_error(parser, invalid_operation_unary, NULL, OP_STRING[op_sum], TYPE_STRING[lh->type]);
            break;
        case token_op_minus:                    
            if (!variable_is_number(lh->type))
                parser_raise_error(parser, invalid_operation_unary, NULL, OP_STRING[op_subtraction], TYPE_STRING[lh->type]);
            lh = parser_operation_multiply(lh, literal_init_with_int(-1));
            break;
        case token_op_NOT:
            if (!variable_compare_type(lh->type, Bool))
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
                result = parser_operation(parser, op_and, lh, rh);
                break;
            default: break;
        }
        *lh = *result;
    }
        
    return lh;
}

Literal* parser_factor(Parser* parser) {
    // factor ::= identifier | string | number | bool | grouped_expression

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
        case token_gp_op:
            return parser_grouped_expression(parser);
        // TODO: implementar bool
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

void parser_unary_operator(Parser* parser) {
    // unary_arithmetic_operator ::= '-' | '+' | '!'
}

void parser_higher_precedence_operator(Parser* parser) {
    // higher_precedence_binary_arithmetic_operator ::= ('/'|'*'|'%'|'&&')
}

void parser_lesser_precedence_operator(Parser* parser) {
    // lesser_precedence_binary_arithmetic_operator ::= ('+'|'-'|'||') 
}

void parser_relational_operator(Parser* parser) {
    // lesser_precedence_binary_logical_operator ::= ('<'|'>'|'<='|'>='|'=='|'!=')
}

Literal* parser_string(Parser* parser) {
    // string ::= '"' ([a-z]|[A-Z])+ '"'

    return literal_init_with_string("test");
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
