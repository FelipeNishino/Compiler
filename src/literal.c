#include "include/literal.h"
#include "include/variable.h"

Literal literal_init(void* value, Type type) {
	return (Literal) {.value = value, .type = type};
}