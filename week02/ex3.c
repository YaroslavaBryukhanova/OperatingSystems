#include<stdio.h>
#include<math.h>

void convert(long long int number, int from, int to);
int convertToDecimal(long long int number, int from);



int main() {

    long long int number = 0;
    int fromBase;
    int toBase;
    long long int decimalNumber;
    scanf("%lld", &number);
    scanf("%d", &fromBase);
    scanf("%d", &toBase);

    if((fromBase>=2 && fromBase<=10) && (toBase>=2 && toBase<=10)){
        if(fromBase==10){
            convert(number, 10, toBase);
        }
        else if(toBase==10){
            decimalNumber = convertToDecimal(number, fromBase);
            printf("%lld", decimalNumber);
        }
        else{
            decimalNumber = convertToDecimal(number, fromBase);
            convert(decimalNumber, 10, toBase);
        }
    }
    else{
        printf("%s", "cannot convert!");
    }
    return 0;
}

void convert(long long int number, int from, int to){
    int remainder;
    char string[256];
    int count=0;
    while (number>0){
        remainder = number%to;
        number = number/to;
        sprintf(&string[count], "%c", (char)remainder);
        count++;
    }
    for(int i=count-1; i>=0; i--) {
        printf("%d",string[i]);
    }
}

int convertToDecimal(long long int number, int from){
    int res = 0;
    int count = 0;
    while(number!=0){
        int n = number%10;
        if(n > from){
            printf("cannot convert!");
            return 0;
        }
        res += n*pow(from, count++);
        number/=10;
    }
    return res;
}