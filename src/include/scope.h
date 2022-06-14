#ifndef COMPILER_SCOPE_H
#define COMPILER_SCOPE_H

#include "hashmap.h"
#include "variable.h"

typedef struct {
	struct hashmap* var_space;
} scope;

scope* scope_init();
bool scope_iter(const void *item, void *udata);
void scope_scan(scope* scope);
void scope_set_variable(scope* scope, variable var);
variable* scope_get_variable(scope* scope, variable var);
variable* scope_get_variable_by_id(scope* scope, const char* identifier);

#endif //COMPILER_SCOPE_H