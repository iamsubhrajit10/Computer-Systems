#include<stdio.h>
extern long x;

int main(void){
    long y=x;
    printf("%lf", y);
}