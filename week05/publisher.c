#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int main() {
    char message[1024];
    int n;
    scanf("%d", &n);
    while (strcmp(message, "exit\n")) {
        mkfifo("/tmp/ex1", 0777);
        scanf("%s", message);

        for (int i = 0; i < n; i++) {
            int fd = open("/tmp/ex1", O_WRONLY);
            write(fd, message, 1024);
            sleep(1);
        }
        close(fd);
    }
    return 0;
}