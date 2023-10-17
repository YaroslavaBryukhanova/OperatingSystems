#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>

int pid;

void handler(int signum) {
    kill(pid, SIGTERM);
    exit(0);
}


int main() {
    FILE* file = fopen("/media/sf_OS/week06/var/run/agent.pid", "r");
    fscanf(file, "%d", &pid);

    if (pid == 0) {
        printf("Error: No agent found.");
        return 0;
    }
    else{
        printf("Agent found.");
    }
    fclose(file);
    char message[256];

    signal(SIGINT, handler);
    while(1) {
        printf("Choose a command {“read”, “exit”, “stop”, “continue”} to send to the agent");
        scanf("%s", message);
        if(strcmp(message, "read")==0){
            kill(pid, SIGUSR1);
        }
        else if(strcmp(message, "exit")==0){
            kill(pid, SIGUSR2);
            exit(0);
        }
        else if(strcmp(message, "stop")==0){
            kill(pid, SIGSTOP);
        }
        else if(strcmp(message, "continue")==0){
            kill(pid, SIGCONT);
        }
    }
}

