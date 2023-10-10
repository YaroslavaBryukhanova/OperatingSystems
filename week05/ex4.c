#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

bool is_prime ( int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0) 
            return false;
    }
    return true;
}

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
int k = 0; 
int c = 0;
int n = 0;

int get_number_to_check () {
    int ret = k;
    if (k != n)
        k ++;
    return ret ;
}

void increment_primes ()    {
    pthread_mutex_lock(&global_lock);
    c++; 
    pthread_mutex_unlock(&global_lock);
}

void * check_primes ( void * arg ) {
    while (1) {
        pthread_mutex_lock(&global_lock);
        int number = get_number_to_check();
        pthread_mutex_unlock(&global_lock);
        if(number == *(int *)arg) {
            return EXIT_SUCCESS;
        }
        if(is_prime(number)) {
            increment_primes();
        }
    }
}

int main(int argc, char *argv[]) {
    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t *threads = malloc(m * sizeof(pthread_t));
    pthread_mutex_init(&global_lock, NULL);
    for(int i=0; i<m; i++) {
        pthread_create(&threads[i], NULL, check_primes, (void *)&k);
    }
    for(int i=0; i<m; i++) {
        pthread_join(threads[i], NULL);
    }
    free(threads);
    pthread_mutex_destroy(&global_lock);
    printf("%d\n", c);
    return 0;
}
