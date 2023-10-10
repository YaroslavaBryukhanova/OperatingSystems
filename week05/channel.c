#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int fd[2];

    if (pipe(fd) == -1) {
        printf("Error");
        return 1;
    }

    int id = getpid();
    fork();

    if (id != getpid()) {
        close(fd[0]);
        while (1) {
            char message[1024];
            scanf("%s", message);
            write(fd[1], message, 1024);
        }
    }
    else {
        close(fd[1]);
        while (1) {
            char message[1024];
            read(fd[0], message, 1024);
            printf("Message: %s\n", message);
        }
    }
    return 0;
}