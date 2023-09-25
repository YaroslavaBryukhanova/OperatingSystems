#include<stdio.h>
#include <stdlib.h>

int const_tri(int* const p, int n);

int main() {
    const int x = 1;
    const int *q = &x;
    int* const p = (int *) calloc(3, sizeof(int));
    *p = x;
    printf("%p\n", p);
    *(p+1) = x;
    printf("%p\n", p+1);
    *(p+2) = 2*x;
    printf("%p\n", p+2);
    printf("%d", const_tri(p, 36));
    free(p);
    return 0;
}

int const_tri(int* const p, int n) {
    if(n==0){
        return 0;
    }
    else if(n==1 || n==2) {
        return 1;
    }
    int temp;
    *p = 0;
    *(p+1) = 1;
    *(p+2) = 1;
    for(int i=0; i<=n-3; i++){
        temp = *(p+2);
        *(p+2) = *p + *(p+1) + *(p+2);
        *p = *(p+1);
        *(p+1) = temp;
    }
    return *(p+2);
}