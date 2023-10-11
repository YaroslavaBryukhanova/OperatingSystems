#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>


int main(char *argv[]) {
    char message[1024];
    int id = atoi(argv[1]);

    while (1) {
        char path[256] = "";
        sprintf(path, "/tmp/ex1/s%d", id);
        int fd = open(path, O_RDONLY);
        read(fd, &message, sizeof(int));
        printf("Message is: %s\n", message);
        close(fd);
    }
    return 0;
}