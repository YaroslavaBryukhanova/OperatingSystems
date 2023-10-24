#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


int main() {
    long int size = 500*1024*1024;
    int fd = open("text.txt", O_RDWR | O_CREAT, 0666);

    ftruncate(fd, size);

    char* connection = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int random_fd = open("/dev/random", O_RDONLY);

    char symbol;
    int count = 0;
    while (count<size) {
        read(random_fd, &symbol, 1);
        if(isprint(symbol)){
            connection[count] = symbol;
            count++;
        }
    }

    long page_size = sysconf(_SC_PAGESIZE);
    int count2 = 0;
    for(int i=0; i<size; i+=page_size){
        for(int j=i; j<i+page_size; j++){
            if(isupper(connection[j])){
                count2++;
                connection[j] = tolower(connection[j]);
            }
        }

    }
    printf("%d",count2);
    munmap(connection, size);
    close(fd);
    close(random_fd);
    return 0;
}