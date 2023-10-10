#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_prime ( int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0) 
            return false;
    }
    return true;
}

int primes_count ( int a, int b) {
    int ret = 0;
    for (int i = a; i < b; i++) 
        if ( is_prime (i) != 0)
            ret ++;
    return ret ;
}

typedef struct prime_request {
    int a, b;
} prime_request ;

void * prime_counter ( void *arg) {

    prime_request *req = (prime_request*)arg;
    int * count = malloc(sizeof(int));
    int start = req->a;
    int finish = req->b;
    *count = primes_count(start, finish);
    return (( void *) count );
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int size = n / m;

    pthread_t *threads = malloc(m * sizeof(pthread_t));
    prime_request *requests = malloc(m * sizeof(prime_request));
    void **number_of_primes = malloc(m * sizeof(void *));
    int start = 0;
    int finish = size;
    for(int i=0; i<m; i++) {
        if(i==0) {
            start +=size * 0;
        }
        else{
            start+=size* 1;
        }
        if(i==0){
            finish +=size * 0;
        }
        else{
            finish +=size* 1;
        }
        prime_request amount = {start, finish};
        requests[i] = amount;
        pthread_create(&threads[i], NULL, prime_counter, (void *)(requests + i));
    }
    int *result;
    int final_result = 0;
    for(int i=0;i<m; i++) {
        pthread_join(threads[i], (void **) &result);
        number_of_primes[i] = result;
    }
    for(int i=0; i<m; i++) {
        final_result +=*(int *)number_of_primes[i];
    }
    printf("%d\n",final_result);
    free(number_of_primes);
    free(threads);
    return 0;
}