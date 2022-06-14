#include "include/literal.h"
#include "include/variable.h"
// #include "include/type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

literal* literal_init(type type) {
	return &(literal) {.value = NULL, .type = type};
}

literal* literal_init_with_int(int val) {
	literal* l = (literal*) malloc(sizeof(literal));
	int *i = (int*)malloc(sizeof(int));
	*i = val;
	l->value = i;
	l->type = Int;
	return l;
}

literal* literal_init_with_float(float val) {
	literal* l = (literal*) malloc(sizeof(literal));
	float *f = (float*)malloc(sizeof(float));
	*f = val;
	l->value = f;
	l->type = Float;
	return l;
}

literal* literal_init_with_string(char* val) {
	literal* l = (literal*) malloc(sizeof(literal));
	int len = strlen(val) - 2;
	char *str = malloc((len + 1) * sizeof(char));
	strncpy(str, &val[1], len);
	l->value = str;
	l->type = String;
	return l;
}

literal* literal_init_with_bool(boolean val) {
	literal* l = (literal*) malloc(sizeof(literal));
	boolean *b = (boolean*)malloc(sizeof(boolean));
	*b = val;
	l->value = b;
	l->type = Bool;
	return l;
}

literal* literal_init_from_var(variable* var) {
	literal* l = (literal*) malloc(sizeof(literal));
	l->value = var->value;
	l->type = var->type;
	return l;
}

literal* literal_cast_to_float(literal* l) {
	if (type_compare(l->type, Float)) return l;
	if (!type_is_number(l->type)) {
		fprintf(stderr, "Bad cast from %s to Float\n", TYPE_STRING[l->type]);
		exit(1);
	}
	float *f = (float*)malloc(sizeof(float));
	*f = *(int*)l->value;

	l->value = f;
	l->type = Float;
	return l;
}

literal* literal_cast_to_int(literal* l) {
	if (type_compare(l->type, Int)) return l;
	if (!type_is_number(l->type)) {
		fprintf(stderr, "Bad cast from %s to Int\n", TYPE_STRING[l->type]);
		exit(1);
	}
	int *i = (int*)malloc(sizeof(int));
	*i = *(float*)l->value;
	l->value = i;
	l->type = Int;
	return l;
}


void _literal_print_int(literal* l) {
	fprintf(stderr, "<Value: {%d}, type: {%s}>\n", *(int*)l->value, TYPE_STRING[l->type]);
}

void _literal_print_float(literal* l) {
	fprintf(stderr, "<Value: {%f}, type: {%s}>\n", *(float*)l->value, TYPE_STRING[l->type]);
}

void _literal_print_bool(literal* l) {
	fprintf(stderr, "<Value: {%s}, type: {%s}>\n", BOOLEAN_STRING[*(boolean*)l->value], TYPE_STRING[l->type]);
}

void _literal_print_string(literal* l) {
	fprintf(stderr, "<Value: {%s}, type: {%s}>\n", (char*)l->value, TYPE_STRING[l->type]);
}

void literal_print(literal* l) {
	switch(l->type) {
		case Int: 
			_literal_print_int(l);
			break;
		case Float:
			_literal_print_float(l);
			break;
		case Bool:
			_literal_print_bool(l);
			break;
		case String:
		case Null:
			_literal_print_string(l);
			break;
	}
}

