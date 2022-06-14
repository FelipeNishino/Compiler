#ifndef COMPILER_LITERAL_H
#define COMPILER_LITERAL_H

#include "variable.h"

typedef struct {
	void* value;
	type type;
} literal;

literal* literal_init(type type);
literal* literal_init_with_int(int val);
literal* literal_init_with_float(float val);
literal* literal_init_with_string(char* val);
literal* literal_init_with_bool(boolean val);
literal* literal_init_from_var(variable* var);
literal* literal_cast_to_float(literal* l);
literal* literal_cast_to_int(literal* l);
void _literal_print_int(literal* l);
void _literal_print_float(literal* l);
void _literal_print_bool(literal* l);
void _literal_print_string(literal* l);
void literal_print(literal* l);

#endif //COMPILER_LITERAL_H