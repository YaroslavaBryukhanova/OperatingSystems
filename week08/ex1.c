#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define password_len 13

int main() {
    FILE* file = fopen("/tmp/ex1.pid","w");
    fprintf(file, "%d", getpid());
    fclose(file);

    char* password = malloc(password_len);
    strcpy(password, "pass:");

    int random = open("/dev/random", O_RDONLY);
    int count = 0;
    for (int i=0; i<password_len-5; i++) {
        char symbol;
        read(random, &symbol, 1);

        while(!isprint(symbol)){
            read(random, &symbol, 1);
        }
        
        password[count+5] = symbol;
        count++;
    }
    close(random);

    while(1){
        sleep(1);
    }
    return 0;
}
