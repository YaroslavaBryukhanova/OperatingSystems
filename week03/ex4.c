#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<float.h>

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*));
void* addDouble(const void* a, const void* b);
void* addInt(const void* a,const void* b);
void* multiplyDouble(const void* a, const void* b);
void* multiplyInt(const void* a, const void* b);
void* maxInt(const void* a, const void* b);
void* maxDouble(const void* a, const void* b);

int main() {
    int arrayOfInt[] = {1,2,3,4,5};
    double arrayOfDouble[] = {2.5, 6.7, 1.1, 8.9, 4.0};

    int initialInt = 0;
    double initialDouble = 0.0;
    int productInt = 1;
    double productDouble = 1.1;

    int resultInt;
    double resultDouble;

    resultInt = *(int*)aggregate(arrayOfInt, sizeof(int), 5, &initialInt, addInt);
    resultDouble = *(double*)aggregate(arrayOfDouble, sizeof(double), 5, &initialDouble, addDouble);


    printf("Addition of Integer:  %d\n", resultInt);
    printf("Addition of Double:  %lf\n", resultDouble);

    resultInt = *(int*)aggregate(arrayOfInt, sizeof(int), 5, &productInt, multiplyInt);
    resultDouble = *(double*)aggregate(arrayOfDouble, sizeof(double), 5, &productDouble, multiplyDouble);
    printf("Multiplication of Integer:  %d\n", resultInt);
    printf("Multiplication of Double:  %lf\n", resultDouble);


    int minInt = INT_MIN;
    double minDouble = -DBL_MAX;
    resultInt = *(int*)aggregate(arrayOfInt, sizeof(int), 5, &minInt, maxInt);
    resultDouble = *(double*)aggregate(arrayOfDouble, sizeof(double), 5, &minDouble, maxDouble);
    printf("Max value of Integer:  %d\n", resultInt); 
    printf("Max value of Double:  %lf\n", resultDouble);

    return 0;
}

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
    
    if (size==sizeof(int)){
        int* array = (int*) base;
        int* value = (int*) initial_value;
        for(int i=0; i<5; i++) {
            value = (int*) opr(value, &array[i]);
        }
        return value;
    }
    else{
        double* array = (double*) base;
        double* value = (double*) initial_value;
        for(int i=0; i<5; i++) {
            value = (double*) opr(value, &array[i]);
        }
        return value;
    }
    
}

void* addInt(const void* a,const void* b){
    int* term1 = (int *)a;
    int* term2 = (int *)b;
    int sum = *term1 + *term2;
    int *sum2 = &sum;
    return sum2;
}

void* addDouble(const void* a, const void* b){
    double* term1 = (double *)a;
    double* term2 = (double *)b;
    double sum = *term1 + *term2;
    double *sum2 = &sum;
    return sum2;
}

void* multiplyDouble(const void* a, const void* b){
    double* term1 = (double *)a;
    double* term2 = (double *)b;
    double product = *term1 * *term2;
    double *product2 = &product;
    return product2;
}

void* multiplyInt(const void* a, const void* b){
    int* term1 = (int *)a;
    int* term2 = (int *)b;
    int product = *term1 * *term2;
    int *product2 = &product;
    return product2;
}

void* maxInt(const void* a, const void* b){
    int* term1 = (int *)a;
    int* term2 = (int *)b;
    int max;
    if(*term1>*term2){
        max = *term1;
    }
    else{
        max = *term2;
    }
    int *max2 = &max;
    return max2;
}

void* maxDouble(const void* a, const void* b){
    double* term1 = (double *)a;
    double* term2 = (double *)b;
    double max;
    if(*term1>*term2){
        max = *term1;
    }
    else{
        max = *term2;
    }
    double *max2 = &max;
    return max2;
}


