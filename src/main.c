#include <stdio.h>
#include "include/iomanager.h"
#include "include/nl.h"

#include "include/variable.h"
#include "include/scope.h"

void printArray(char* a, int n) {
    int i = 0;
    for (i = 0; i < n; i++)
        printf(" %d ", a[i]);
}

int main(int argc, const char *argv[]) {
    if (0) {


    char *input;
    
    if (argc > 1)
        nl_compile_file(argv[1]);
    else {
        input = getInputFromStdin();
        nl_compile(input);
    }
    }

    Variable* var1 = variable_init_with_id("var1");

    Scope* scope = scope_init();
    scope_set_variable(scope, *var1);
    Variable* var2 = scope_get_variable_by_id(scope, "var1");
    int val = 10;
    var2->value = &val;
    printf("valor var1: %d\n", *(int*)var2->value);
    scope_set_variable(scope, *var2);
    var1 = scope_get_variable_by_id(scope, "var1");
    printf("valor var1: %d\n", *(int*)var1->value);

    return 0;
}
