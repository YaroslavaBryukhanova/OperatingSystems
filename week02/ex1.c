#include<stdio.h>
#include<limits.h>
#include<float.h>

int main() {

    int intVar = INT_MAX;
    unsigned short int  unsignedShortIntVar = USHRT_MAX;
    signed long int signedLongIntVar = LONG_MAX;
    float floatVar = FLT_MAX;
    double doubleVar = DBL_MAX;
    printf("size: %d, value: %d\n", (int)(sizeof(intVar)), intVar);
    printf("size: %d, value: %d\n", (int)(sizeof(unsignedShortIntVar)), unsignedShortIntVar);
    printf("size: %d, value: %d\n", (int)(sizeof(signedLongIntVar)), signedLongIntVar);
    printf("size: %d, value: %f\n", (int)(sizeof(floatVar)), floatVar);
    printf("size: %d, value: %lf\n", (int)(sizeof(doubleVar)), doubleVar);
    return 0;
}