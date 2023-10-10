#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define N 3

struct Thread {
    pthread_t id;
    int i;
    char message[256];
};


void *thread_start(void *threads) {
    struct Thread *threads_data = (struct Thread *)threads;
    printf("%li %s\n", threads_data->id, threads_data->message);
    return NULL;
}

int main() {
    struct Thread threads[N];

    for(int i=0; i<N; i++) {
        threads[i].i = i;
        sprintf(threads[i].message, "Hello from thread %d", i);
        pthread_create(&threads[i].id, NULL, thread_start, (void *)(threads + i));
        printf("Thread %i is created\n", threads[i].i);
        pthread_join(threads[i].id, NULL);
    }
    return 0;
}