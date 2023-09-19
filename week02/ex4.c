#include<stdio.h>

int count(char string[256], char symbol);
void countAll(char string[256]);

int main() {
    char string[256];
    int symbol;
    for (int i=0; (symbol = getchar()) != '\n'; i++ ) {
        string[i] = symbol;
    }
    countAll(string);
    return 0;
}

int count(char string[256], char symbol) {
    int res = 0, len;
    for (int i = 0; i < strlen(string); i++) {
        if (symbol == string[i])
            res++;
    }
    return res;
}

void countAll(char string[256]) {
    for (int i = 0; i < strlen(string) - 1; i++) {
        printf("%c: %d", string[i], count(string, string[i]));
        if (i < strlen(string) - 2){
            printf(", ");
        }
    }
}