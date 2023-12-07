
#include <stdio.h>
extern long x;

typedef union {
    long l;
    double d;
} LongDoubleUnion;

int main(void) {
    LongDoubleUnion u;
    u.l = x;
    
    printf("%lf \n", u.d);
    return 0;
}