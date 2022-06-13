#include "include/literal.h"
#include "include/variable.h"
// #include "include/type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Literal* literal_init(Type type) {
	return &(Literal) {.value = NULL, .type = type};
}

Literal* literal_init_with_int(int val) {
	Literal* l = (Literal*) malloc(sizeof(Literal));
	int *i = (int*)malloc(sizeof(int));
	*i = val;
	l->value = i;
	l->type = Int;
	return l;
}

Literal* literal_init_with_float(float val) {
	Literal* l = (Literal*) malloc(sizeof(Literal));
	float *f = (float*)malloc(sizeof(float));
	*f = val;
	l->value = f;
	l->type = Float;
	return l;
}

Literal* literal_init_with_string(char* val) {
	Literal* l = (Literal*) malloc(sizeof(Literal));
	int len = strlen(val) - 2;
	char *str = malloc((len + 1) * sizeof(char));
	strncpy(str, &val[1], len);
	l->value = str;
	l->type = String;
	return l;
}

Literal* literal_init_with_bool(Boolean val) {
	Literal* l = (Literal*) malloc(sizeof(Literal));
	Boolean *b = (Boolean*)malloc(sizeof(Boolean));
	*b = val;
	l->value = b;
	l->type = Bool;
	return l;
}

Literal* literal_init_from_var(Variable* var) {
	Literal* l = (Literal*) malloc(sizeof(Literal));
	l->value = var->value;
	l->type = var->type;
	return l;
}

void literal_print(Literal* l) {
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

void _literal_print_int(Literal* l) {
	fprintf(stderr, "<Value: {%d}, Type: {%s}>\n", *(int*)l->value, TYPE_STRING[l->type]);
}

void _literal_print_float(Literal* l) {
	fprintf(stderr, "<Value: {%f}, Type: {%s}>\n", *(float*)l->value, TYPE_STRING[l->type]);
}

void _literal_print_bool(Literal* l) {
	fprintf(stderr, "<Value: {%s}, Type: {%s}>\n", BOOLEAN_STRING[*(Boolean*)l->value], TYPE_STRING[l->type]);
}

void _literal_print_string(Literal* l) {
	fprintf(stderr, "<Value: {%s}, Type: {%s}>\n", (char*)l->value, TYPE_STRING[l->type]);
}
