#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union test_t {
    int foo;
    short bar;
    char baz;
} test;

int main() {
    test foo;

    memset(&foo, 0, sizeof(test));

    printf("%d %d %d\n\n", foo.foo, foo.bar, foo.baz);

    foo.foo = 42;
    foo.bar = 13;

    printf("%d %d %d\n\n", foo.foo, foo.bar, foo.baz);

    printf("%d\n", sizeof(test));
    return 0;
}
