#include "include/hashmap.h"
#include "include/variable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Variable* variable_init() {
    Variable* var = calloc(1, sizeof(Variable));
    return var;
}

Variable* variable_init_with_id(const char* id) {
    Variable* var = calloc(1, sizeof(Variable));
    var->identifier = id;
    return var;   
}

void _variable_print_int(Variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%d, Type:%s>\n", v.identifier, v.isConstant, *(int*)v.value, TYPE_STRING[v.type]);
}

void _variable_print_float(Variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%f, Type:%s>\n", v.identifier, v.isConstant, *(float*)v.value, TYPE_STRING[v.type]);
}

void _variable_print_bool(Variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%s, Type:%s>\n", v.identifier, v.isConstant, BOOLEAN_STRING[*(Boolean*)v.value], TYPE_STRING[v.type]);
}

void _variable_print_string(Variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%s, Type:%s>\n", v.identifier, v.isConstant, (char*)v.value, TYPE_STRING[v.type]);
}

void _variable_print_uninitialized(Variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%s, Type:%s>\n", v.identifier, v.isConstant, (char*)v.value, TYPE_STRING[v.type]);
}

void variable_print(Variable v) {
    if (v.value == NULL) {
        _variable_print_uninitialized(v);
        return;
    }
    switch(v.type) {
        case Int: 
            _variable_print_int(v);
            break;
        case Float:
            _variable_print_float(v);
            break;
        case Bool:
            _variable_print_bool(v);
            break;
        case String:
        case Null:
            _variable_print_string(v);
            break;
    }
}

// void variable_print(Variable v) {
//     if (v.value)
//         fprintf(stderr, "(Id: {%s}, Type: {%s}, constant: {%d}, value: {%d})\n", v.identifier, TYPE_STRING[v.type], v.isConstant, *((int*)v.value));
//     else
//         fprintf(stderr, "(Id: {%s}, Type: {%s}, constant: {%d}, value: {none})\n", v.identifier, TYPE_STRING[v.type], v.isConstant);
// }

int variable_compare(const void *a, const void *b, void *udata) {
    const Variable *var_a = a;
    const Variable *var_b = b;
    return strcmp(var_a->identifier, var_b->identifier);
}

bool variable_iter(const void *item, void *udata) {
    const Variable *var = item;
    // printf("<var={%s}, var_type={%d}, var_value={%s}>\n", var->identifier, var->Type, var->code);
    printf("<var={%s}>\n", var->identifier);
    return true;
}

uint64_t variable_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const Variable *var = item;
    return hashmap_sip(var->identifier, strlen(var->identifier), seed0, seed1);
}

// void variable_new_scope(Scope *s) {
// 	s = hashmap_new(
//         sizeof(struct Variable),
//         0,
//         0,
//         0,
//         variable_hash,
//         variable_compare,
//         NULL,
//         NULL);
// }
