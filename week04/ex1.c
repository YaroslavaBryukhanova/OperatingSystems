#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int main() {

    clock_t time_start = clock();

    pid_t process1;
    pid_t process2;

    
    process1 = fork();
    clock_t time1_start = clock();

    if (process1 == 0) {
        clock_t time1_end = clock();
        printf("process1 %d %d %f ms\n", getpid(), getppid(), (float) (time1_end - time1_start)*1000/ CLOCKS_PER_SEC);
        exit(0);
    } 
    else {
        process2 = fork();
        clock_t time2_start = clock();

        if (process2 == 0){
            clock_t time2_end = clock();
            printf("process2 %d %d %f ms\n", getpid(), getppid(), (float) (time2_end - time2_start) * 1000/ CLOCKS_PER_SEC);
            exit(0);
        }
        else {
            for(int i=0; i<2; i++) {
                wait(NULL);
            }
            clock_t time_end = clock();
            printf("main process %d %d %f ms\n", getpid(), getppid(), (float) (time_end - time_start)*1000 / CLOCKS_PER_SEC);
        }
    }
}