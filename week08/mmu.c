#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    pid_t mmu_pid;
    int disk_accesses;
};

void handle_loaded_page();
void handle_nonvalid_page(int page);

int pages_number;
pid_t pid;
struct PTE *table;


int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "You need to enter number of pages, reference string, pager pid\n");
        exit(EXIT_FAILURE);
    }
    pages_number = atoi(argv[1]);
    char *reference_string = malloc(sizeof(char) * argc - 3);
    pid = atoi(argv[argc - 1]);

    for (int i=2; i<argc-1; i++) {
        strcat(reference_string, argv[i]);
    }

    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(fd, sizeof(struct PTE)* pages_number);
    table = (struct PTE *)mmap(NULL, pages_number * sizeof(struct PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if(table==MAP_FAILED) {
        perror("Mmap failed");
        exit(EXIT_FAILURE);
    }
    table->mmu_pid = getpid();
    printf("-------------------------\n");
    printf("Initialized page table\n");
    printf("Page table:\n");
    for(int i=0; i< pages_number; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",i, table[i].valid, table[i].frame, table[i].dirty, table[i].referenced);
    }
    printf("-------------------------\n");
    for (int i=0; i<strlen(reference_string); i+=2) {
        char mode = reference_string[i];
        int page = reference_string[i+1] - '0';

        if (mode == 'R') {
            printf("Read Request for page %d\n", page);
            if (!table[page].valid) {
                printf("It is not a valid page --> page fault\n");
                handle_nonvalid_page(page);
            } else {
                printf("It is a valid page\n");
            }
        }
        if (mode == 'W') {
            printf("Write Request for page %d\n", page);    
            table[page].dirty = true;
        } else {
            printf("Read Request for page %d\n", page);
        }

        printf("Page table\n");
        for (int count = 0; count < pages_number; count++) {
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", count, table[count].valid, table[count].frame, table[count].dirty, table[count].referenced);
        }
        printf("-------------------------\n");
    }
    printf("Done all requests.\n");

    munmap(table, pages_number * sizeof(struct PTE));
    close(fd);
    printf("MMU sends SIGUSR2 to the pager. \nMMU terminates.\n");
    kill(pid, SIGUSR2);
    return 0;
}

void handle_nonvalid_page(int page) {
    printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
    if(!table[page].valid) {
        table[page].referenced = 1;
        kill(pid, SIGUSR1);
        signal(SIGCONT, handle_loaded_page);
        pause();
    }
}

void handle_loaded_page() {
    printf("MMU resumed by SIGCONT signal from pager\n");
}