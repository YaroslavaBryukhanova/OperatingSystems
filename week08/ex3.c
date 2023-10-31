#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>

#define memory_size 10*1024*1024

int main() {
    for(int i=0; i<10; i++) {
        void *memory = malloc(memory_size);
        memset(memory, '0', memory_size);
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        printf("%d\n", usage.ru_maxrss);
        sleep(1);
    }
    return 0;
}