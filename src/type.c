#include "include/type.h"

int type_compare(Type t1, Type t2) {
    return t1 == t2;
}

int type_is_number(Type t) {
    return type_compare(t, Int) || type_compare(t, Float);
}

Boolean bool_init_from_int(int r) {
    return (r?True:False);
}

int number_is(Number n, number_discriminant d) {
    return n.discriminant == d;
}

Number number_init_with_int(int i) {
    Number n;
    n.discriminant = I;
    n.i = i;
    return n;  
}

Number number_init_with_float(float f) {
    Number n;
    n.discriminant = F;
    n.f = f;
    return n;  
}
