#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main() {
    char input[256];
    while (1) {
        printf("Write the command: ");
        scanf("%s", input);
        if(strcmp(input, "exit") == 0){
            return 0;
        }
        else{
            system(input);
        }
    }
    return 0;    
}