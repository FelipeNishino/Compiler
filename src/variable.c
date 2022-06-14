#include "include/hashmap.h"
#include "include/variable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

variable* variable_init() {
    variable* var = calloc(1, sizeof(variable));
    return var;
}

variable* variable_init_with_id(const char* id) {
    variable* var = calloc(1, sizeof(variable));
    var->identifier = id;
    return var;   
}

void _variable_print_int(variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%d, Type:%s>\n", v.identifier, v.is_constant, *(int*)v.value, TYPE_STRING[v.type]);
}

void _variable_print_float(variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%f, Type:%s>\n", v.identifier, v.is_constant, *(float*)v.value, TYPE_STRING[v.type]);
}

void _variable_print_bool(variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%s, Type:%s>\n", v.identifier, v.is_constant, BOOLEAN_STRING[*(boolean*)v.value], TYPE_STRING[v.type]);
}

void _variable_print_string(variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%s, Type:%s>\n", v.identifier, v.is_constant, (char*)v.value, TYPE_STRING[v.type]);
}

void _variable_print_uninitialized(variable v) {
    fprintf(stderr, "<Id:%s, Constant:%d, Value:%s, Type:%s>\n", v.identifier, v.is_constant, (char*)v.value, TYPE_STRING[v.type]);
}

void variable_print(variable v) {
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

// void variable_print(variable v) {
//     if (v.value)
//         fprintf(stderr, "(Id: {%s}, Type: {%s}, constant: {%d}, value: {%d})\n", v.identifier, TYPE_STRING[v.type], v.is_constant, *((int*)v.value));
//     else
//         fprintf(stderr, "(Id: {%s}, Type: {%s}, constant: {%d}, value: {none})\n", v.identifier, TYPE_STRING[v.type], v.is_constant);
// }

int variable_compare(const void *a, const void *b, void *udata) {
    const variable *var_a = a;
    const variable *var_b = b;
    return strcmp(var_a->identifier, var_b->identifier);
}

bool variable_iter(const void *item, void *udata) {
    const variable *var = item;
    // printf("<var={%s}, var_type={%d}, var_value={%s}>\n", var->identifier, var->Type, var->code);
    printf("<var={%s}>\n", var->identifier);
    return true;
}

uint64_t variable_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const variable *var = item;
    return hashmap_sip(var->identifier, strlen(var->identifier), seed0, seed1);
}

// void variable_new_scope(Scope *s) {
// 	s = hashmap_new(
//         sizeof(struct variable),
//         0,
//         0,
//         0,
//         variable_hash,
//         variable_compare,
//         NULL,
//         NULL);
// }
