#ifndef COMPILER_SCOPE_H
#define COMPILER_SCOPE_H

#include "hashmap.h"
#include "variable.h"

typedef struct Scope {
	struct hashmap* var_space;
} Scope;

Scope* scope_init();
bool scope_iter(const void *item, void *udata);
void scope_scan(Scope* scope);
void scope_set_variable(Scope* scope, Variable var);
Variable* scope_get_variable(Scope* scope, Variable var);
Variable* scope_get_variable_by_id(Scope* scope, const char* identifier);

#endif //COMPILER_SCOPE_H