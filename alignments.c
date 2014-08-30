#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct test_t {
    unsigned int four;
    unsigned short two;
    unsigned char reserved[2];
} test;

int main() {
    /* I'm clearly a dinosaur so I use the C89 requirement of not declaring vars in for initializers, those 4 bytes of stack are important! */
    /* I also try to declare at the beginning of blocks! Dinosaur! Rawr! */
    test foo;
    /* Never do this, this is for science. In real life this is not smart */
    unsigned char *bytes;
    unsigned int i;

    printf("%d\n", sizeof(test));

    bytes = (unsigned char *)&foo;

    memset(&foo, 0, sizeof(test));

    foo.four = 0xdeadbeef;
    foo.two = 0xcafe;

    /* You may be wondering why this works, because pointers and low level memory management are awesome */
    for (i = 0; i < sizeof(test); ++i) {
        printf("%x ", bytes[i]);
    }

    printf("\n");
    return 0;
}
