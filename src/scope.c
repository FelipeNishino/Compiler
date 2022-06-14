#include "include/scope.h"
#include <stdlib.h>

scope* scope_init() {
	scope* s = calloc(1, sizeof(scope));
	s->var_space = hashmap_new(
        sizeof(variable),
        0,
        0,
        0,
        variable_hash,
        variable_compare,
        NULL,
        NULL);

	return s;
}

bool scope_iter(const void *item, void *udata) {
    const variable *var_item = item;
    variable_print(*var_item);
    return 1;
}

void scope_scan(scope* s) {
    hashmap_scan(s->var_space, scope_iter, NULL);
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

void scope_set_variable(scope* s, variable var) {
	hashmap_set(s->var_space, &var);
}

variable* scope_get_variable(scope* s, variable var) {
	return (variable*) hashmap_get(s->var_space, &var);
}

variable* scope_get_variable_by_id(scope* s, const char* identifier) {
	return (variable*) hashmap_get(s->var_space, &(variable){ .identifier=identifier });
}
