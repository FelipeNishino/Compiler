#include "include/scope.h"
#include <stdlib.h>

Scope* scope_init() {
	Scope* scope = calloc(1, sizeof(Scope));
	scope->var_space = hashmap_new(
        sizeof(struct Variable),
        0,
        0,
        0,
        variable_hash,
        variable_compare,
        NULL,
        NULL);

	return scope;
}

bool scope_iter(const void *item, void *udata) {
    const Variable *var_item = item;
    variable_print(*var_item);
    return 1;
}

void scope_scan(Scope* scope) {
    hashmap_scan(scope->var_space, scope_iter, NULL);
}

    //     // 

    // printf("\n-- iterate over all users (hashmap_scan) --\n");
    // hashmap_scan(map, cfr_iter, NULL);

    // printf("\n-- iterate over all users (hashmap_iter) --\n");
    // size_t iter = 0;
    // void *item;
    // while (hashmap_iter(map, &iter, &item)) {
    //     const struct user *user = item;
    //     printf("%s (age=%d)\n", user->name, user->age);
    // }
    // hashmap_free(map);
    // return 0;

void scope_set_variable(Scope* scope, Variable var) {
	hashmap_set(scope->var_space, &var);
}

Variable* scope_get_variable(Scope* scope, Variable var) {
	return (Variable*) hashmap_get(scope->var_space, &var);
}

Variable* scope_get_variable_by_id(Scope* scope, const char* identifier) {
	return (Variable*) hashmap_get(scope->var_space, &(Variable){ .identifier=identifier });
}
