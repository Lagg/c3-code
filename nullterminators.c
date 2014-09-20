#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/* The size of our example block.
 * Yes it'll be heap allocated despite
 * being constant. Deal with it.
 */
#define HEAP_BLOCK_SIZE 10

char git_random_readable_char() {
    int ch = 0;

    do {
        ch = rand() % 126;
    } while (ch > 126 || ch < 32);

    return ch;
}

int main() {
    /* Heap allocation with 4 byte (probably) address size */
    char *foo = (char *)malloc(HEAP_BLOCK_SIZE);
    int i;

    /* Set up random with the worst seed in the world */
    srand(time(NULL));

    /* Initializing the $HEAP_BLOCK_SIZE bytes of space allocated at foo with random *readable* chars
     * (because printing random characters in uninitialized RAM (which is where heartbleed got data from) is mean
     */
    memset(foo, '!', HEAP_BLOCK_SIZE);
    for (i = 0; i < HEAP_BLOCK_SIZE; ++i) {
        foo[i] = git_random_readable_char();
    }

    /* Write string of 2 bytes + 1 for the null terminator to foo's space */
    memcpy(foo, "hi", 3);

    printf("What it LOOKS like: %s\n\n", foo);

    /* Remove null terminator by overwriting it with 'p' so that printf reads
     * off the end into space which would normally be uninitialized if we
     * hadn't filled it in the loop earlier.
     */
    foo[2] = 'p';
    printf("What it IS: %s\n", foo);

    printf("\n\nConclusion: Computar is hard\n");

    /* Because it matters. IT MATTERS. */
    free(foo);

    return 0;
}
