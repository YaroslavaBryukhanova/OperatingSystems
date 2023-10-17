#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

void signal_handler (int signum) {
    if(signum == SIGUSR1) {
        FILE* file = fopen("/media/sf_OS/week06/text.txt", "r");
        char message[256];
        fgets(message, sizeof(message), file);
        printf("%s\n", message);
        fclose(file);
    }
    else if(signum == SIGUSR2) {
        printf("Process terminating...\n");
        exit(0);
    }
}



int main() {

    FILE *file;
    file = fopen("/media/sf_OS/week06/var/run/agent.pid", "w");
    fprintf(file, "%d", getpid());
    fclose(file);
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    while (1) {
        sleep(1);
    }
    return 0;
}

