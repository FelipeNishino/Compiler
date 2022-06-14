#include "include/type.h"

int type_compare(type t1, type t2) {
    return t1 == t2;
}

int type_is_number(type t) {
    return type_compare(t, Int) || type_compare(t, Float);
}

boolean bool_init_from_int(int r) {
    return (r?True:False);
}

int number_is(number n, number_discriminant d) {
    return n.discriminant == d;
}

number number_init_with_int(int i) {
    number n;
    n.discriminant = I;
    n.i = i;
    return n;  
}

number number_init_with_float(float f) {
    number n;
    n.discriminant = F;
    n.f = f;
    return n;  
}
