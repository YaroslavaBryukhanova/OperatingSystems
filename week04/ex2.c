#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
    int u[120];
    int v[120];
    for(int i=0; i<120; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }
    int n;
    scanf("%d", &n);
    FILE* file = fopen("temp.txt", "w+");
    pid_t process[n];

    for(int i=0; i<n; i++) {
        process[i] = fork();
        int sum = 0;
        int step = 120/n;
        if(process[i] == 0) {
            for(int j = i*step; j<(i*step)+step; j++) {
                sum += u[j] * v[j];
            }
            fprintf(file, "%d\n", sum);
            fclose(file);
            exit(0);
        }
    }

    for(int i = 0; i < n; i++){
        waitpid(process[i], NULL, 0);
    }

    int temp = 0;
    int result = 0;
    file = fopen("temp.txt", "r");
    for(int i=0; i<n; i++){
        fscanf(file, "%d", &temp);
        result += temp;
        printf("%d\n", temp);
    }
    printf("Final result: %d\n", result);
    fclose(file);
    return 0;
}