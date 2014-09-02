/* This is a bit different from the code in the video in terms of naming and
 * comments but otherwise is overall the same layout. Should be able to follow
 * along.
 */

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

    /* Never do this, this is for science. In real life this is not smart */
    unsigned char *bytes;

    unsigned int i;
    test foo;

    printf("%d\n", sizeof(test));

    /* Set the bytes pointer to the address of the struct instance */
    bytes = (unsigned char *)&foo;

    /* Set a block of memory equal to the size of the struct to 0 starting at the
     * address given by &foo
     */
    memset(&foo, 0, sizeof(test));

    foo.four = 0xdeadbeef;
    foo.two = 0xcafe;

    /* Here we iterate each byte of memory where the struct instance is located,
     * this works because the bytes pointer is set to the address of it and thus
     * enables us to look through it byte by byte (or char by char)
     */
    for (i = 0; i < sizeof(test); ++i) {
        printf("%x ", bytes[i]);
    }

    printf("\n");
    return 0;
}
