#include<stdio.h>
#include <string.h>

int main() {

    char string[256];
    int count;
    int symbol;
    for (count = 0; (symbol = getchar()) != '.' && symbol != '\n'; count++ ) {
        string[count] = symbol;
    }

    putchar('"');
    for(int i=count-1; i>=0; i--){
        putchar(string[i]);
    }
    putchar('"');
    return 0;
}

