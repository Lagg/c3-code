#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union test_t {
    int foo;
    short bar;
    char baz;
} test;

typedef union ptrint_t {
    int integer;
    char *string;
} ptrint;

int main() {
    test foo;

    memset(&foo, 0, sizeof(test));

    printf("%d %d %d\n\n", foo.foo, foo.bar, foo.baz);

    foo.foo = 42;
    foo.bar = 13;

    printf("%d %d %d\n\n", foo.foo, foo.bar, foo.baz);

    printf("%d\n", sizeof(test));

    /* Here begins non-video material. */
    /* (note: braces are just ways of indicating a generic block) */
    {
        ptrint cheeseburger;
        const char *teststring = "MogDogSixtyLicks";

        memset(&cheeseburger, 0, sizeof(ptrint));

        /* Say I wanted to have a variable where I could store either a number 
         * or string but only needed to have one at a time.
         */
        
        /* Here I'll show the address and size of the ptrint instance. Remember this. */
        printf("\nptrint: Address: %p, size: %d\n", (void *)&cheeseburger, sizeof(ptrint));

        /* So I decide to store an integer */
        cheeseburger.integer = 42;

        printf("  int: Address: %p, size: %d, content: 0x%2x\n", (void *)&cheeseburger.integer, sizeof(cheeseburger.integer), cheeseburger.integer);

        /* Now, I no longer need to store an integer, I decide to store a string instead (remember: pointers on 32 bit systems are 32 bits or 4 bytes long, just like an int) */
        cheeseburger.string = (char *)teststring;

        printf("  string: Address: %p, size: %d, content: %s\n", (void *)&cheeseburger.string, sizeof(cheeseburger.string), cheeseburger.string);

        /* Remember the address of the string above and the content of the integer before? Look at this and tell me what it means. */
        printf("\nstring address: 0x%2p, integer value: 0x%02x\n", cheeseburger.string, cheeseburger.integer);
    }

    return 0;
}
