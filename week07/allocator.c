#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void allocateFirstFit(unsigned int adrs, int size);
void allocateBestFit(unsigned int adrs, int size);
void allocateWorstFit(unsigned int adrs, int size);
void clear(unsigned int adrs);

unsigned int memory[10000000];
int N = 10000000;

int main() {
    FILE* file; 
    int count = 0;
    for(int i=0; i<3; i++) {
        count = 0;
        for(int i=0; i<N; i++) {
            memory[i] = 0;
        }
        
        clock_t time_start = clock();

        file = fopen("queries.txt", "r");
        char str[256];
        while(strcmp(fgets(str, 256, file), "end")!=0) {
            count++;
            char *token = strtok(str," ");
            int count_of_words = 0;
            char action[256] = "";
            int value = 0;
            int size = 0;
            while (token != NULL) {
                if(count_of_words==0) {
                    strcpy(action, token);
                }
                else if(count_of_words==1) {
                    value = atoi(token);
                }
                else {
                    size = atoi(token);
                }
                count_of_words++;
                token = strtok(NULL, " ");
            }
            if(i==0) {
                if(strcmp(action, "allocate")==0) {
                    allocateFirstFit(value, size);
                }
                else {
                    clear(value);
                }
            }
            else if(i==1) {
                if(strcmp(action, "allocate")==0) {
                    allocateBestFit(value, size);
                }
                else {
                    clear(value);
                }
            } 
            else {
                if(strcmp(action, "allocate")==0) {
                    allocateWorstFit(value, size);
                }
                else {
                    clear(value);
                }
            }     
        }
        clock_t time_end = clock();
        printf("Time: %lf Throughput: %lf\n", (time_end-time_start)/1e5, count/((time_end-time_start)/1e5));
        fclose(file);
    }
    return 0;
}

void allocateFirstFit(unsigned int adrs, int size) {
    if(size>N || size<1) {
        printf("Error: wrong size value");
    }

    int first_index = -1;
    int count = 0;
    for(int i=0; i<N; i++) {
        if (memory[i] == 0) {
            if(count == 0) {
                first_index = i;
            }
            count++;
            if(count == size){
                break;
            }
        }
        else {
            count = 0;
        }
    }


    for(int i=first_index; i<first_index + size; i++) {
        memory[i] = adrs;
    }
}

void allocateBestFit(unsigned int adrs, int size) {
    if(size>N || size<1) {
        printf("Error: wrong size value");
    }

    int first_index = -1;
    int count = 0;
    int min_memory_alloc = N;
    for(int i=0; i<N; i++){
        if(memory[i]==0 && i!=N-1){
            count++;
        }
        else{
            if(i==N-1) {
                count++;
            }
            if(count >= size){
                if(count <= min_memory_alloc) {
                    min_memory_alloc = count;
                    first_index = i - count;
                    if(i==N-1) {
                        first_index++;
                    }
                }
            }
            count = 0;
        }
    }
    for(int i=first_index; i<first_index + size; i++) {
        memory[i] = adrs;
    }
}

void allocateWorstFit(unsigned int adrs, int size) {
    if(size>N || size<1) {
        printf("Error: wrong size value");
    }
    int first_index = -1;
    int count = 0;
    int max_memory_alloc = -1;
    for(int i=0; i<N; i++){
        if(memory[i]==0 && i!=N-1){
            count++;
        }
        else{
            if(i==N-1) {
                count++;
            }
            if(count >= size){
                if(count > max_memory_alloc) {
                    max_memory_alloc = count;
                    first_index = i - count;
                    if(i==N-1) {
                        first_index++;
                    }
                }
            }
            count = 0;
        }
    }

    if(max_memory_alloc == -1) {
        return;
    }

    for(int i = first_index; i < first_index + size; i++) {
        memory[i] = adrs;
    }
}

void clear(unsigned int adrs) {
    for(int i=0; i<N; i++) {
        if(memory[i]==adrs) {
            memory[i] = 0;
        }
    }
}