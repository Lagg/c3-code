#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This macro is going to blow your mind */
#define PRINTSIZE(s, counter) if (counter != NULL) { *(unsigned int *)counter += sizeof(s); } printf("%s: %d\n", #s, sizeof(s))

/* BEGIN Minix source code snippet */

#define M1                 1
#define M3                 3
#define M4                 4
#define M3_STRING         14

/*  Structs taken from minix 3 source code. Yes I know it's ugly, but such is the nature of pure computer scientists */
typedef struct {int m1i1, m1i2, m1i3; char *m1p1, *m1p2, *m1p3;} mess_1;
typedef struct {int m2i1, m2i2, m2i3; long m2l1, m2l2; char *m2p1;} mess_2;
typedef struct {int m3i1, m3i2; char *m3p1; char m3ca1[M3_STRING];} mess_3;
typedef struct {long m4l1, m4l2, m4l3, m4l4, m4l5;} mess_4;
typedef struct {short m5c1, m5c2; int m5i1, m5i2; long m5l1, m5l2, m5l3;}mess_5;
typedef struct {int m7i1, m7i2, m7i3, m7i4; char *m7p1, *m7p2;} mess_7;
typedef struct {int m8i1, m8i2; char *m8p1, *m8p2, *m8p3, *m8p4;} mess_8;

typedef struct {
  int m_source;			/* who sent the message */
  int m_type;			/* what kind of message is it */
  union {
	mess_1 m_m1;
	mess_2 m_m2;
	mess_3 m_m3;
	mess_4 m_m4;
	mess_5 m_m5;
	mess_7 m_m7;
	mess_8 m_m8;
  } m_u;
} message;

/* END Minix source code snippet */

int main() {
    unsigned int counter = 0;
    message foo;

    /* The size of the union in the foo instance */
    PRINTSIZE(foo.m_u, NULL);

    printf("\n\n");

    /* The sizes of each struct in the union */
    PRINTSIZE(mess_1, &counter);
    PRINTSIZE(mess_2, &counter);
    PRINTSIZE(mess_3, &counter);
    PRINTSIZE(mess_4, &counter);
    PRINTSIZE(mess_5, &counter);
    PRINTSIZE(mess_7, &counter);
    PRINTSIZE(mess_8, &counter);

    /* The total allocated space if this wasn't a union */
    printf("\n\n%d\n", counter);

    return 0;
}
