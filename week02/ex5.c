#include<stdio.h>

int main() {
    int n;
    printf("4:  %d\n", tribonacci(4));
    printf("36:  %d\n", tribonacci(36));
    return 0;
}

int tribonacci(int n) {
    int temp;
    int n0 = 0;
    int n1 = 1;
    int n2 = 1;
    if(n==0){
        return 0;
    }
    else if(n==1 || n==2){
        return 1;
    }
    for(int i=0; i<=n-3; i++){
        temp = n2;
        n2 = n2 + n1 + n0;
        n0 = n1;
        n1 = temp;
    }
    return n2;
}