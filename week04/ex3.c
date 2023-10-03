#include <stdlib.h>
#include <unistd.h>

int main(int n) {
    for(int i=0; i<n; i++) {
        fork();
        sleep(5);
    }
    return 0;
}