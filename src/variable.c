#include "include/hashmap.h"
#include "include/variable.h"
#include <stdio.h>
#include <string.h>

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

void variable_new_scope(Scope *s) {
	s = hashmap_new(
        sizeof(struct Variable),
        0,
        0,
        0,
        variable_hash,
        variable_compare,
        NULL,
        NULL);
}
