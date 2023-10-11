#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char message[1024];
    int n = atoi(argv[1]);
    while (strcmp(message, "exit")) {
        mkfifo("/tmp/ex1", 0777);
        scanf("%s", message);

        for (int i = 0; i < n; i++) {
            pid_t pid = fork();
            if (pid == 0){
                char path[256] = "";
                sprintf(path, "/tmp/ex1/s%d", i + 1);
                int fd = open(path, O_WRONLY);
                write(fd, message, 1024);
                sleep(1);
                close(fd);
                exit(0);
            }
        }
    }
    return 0;
}