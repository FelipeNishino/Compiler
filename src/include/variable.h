#include "hashmap.h"

typedef struct hashmap Scope;

typedef enum Type {
	Int, Float, Bool, String
}Type;

typedef struct Variable {
	char* identifier;
	Type type;
	void* value;
} Variable;

int variable_compare(const void *a, const void *b, void *udata);
bool variable_iter(const void *item, void *udata);
uint64_t variable_hash(const void *item, uint64_t seed0, uint64_t seed1);
void variable_new_scope(Scope *s);