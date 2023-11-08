#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 8

void sigusr1_handler();

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    pid_t mmu_pid;
    int disk_accesses;
};

int pages_number;
int frames;
int max_pages = 100;
int max_frames = 100;
struct PTE *table;
char** disk;
int current_frame = 0;
char **RAM;
int disk_accesses = 0;
char *reference_string;


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "You need to enter number of pages, number of frames\n");
        exit(EXIT_FAILURE);
    }

    pages_number = atoi(argv[1]);
    frames = atoi(argv[2]);

    pid_t pid = getpid();
    FILE* file = fopen("/tmp/ex2/pager_pid", "w");
    fprintf(file, "%d", pid);
    fclose(file);

    if(pages_number > max_pages || frames > max_frames) {
        fprintf(stderr, "Error: number of pages or frames exceeds maximum\n");
        exit(EXIT_FAILURE);
    }

    printf("Create a page table\n");
    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT);
    
    printf("Shorten file to the size of a page table");
    ftruncate(fd, sizeof(struct PTE) * pages_number);

    printf("Map page table to the memory");
    table = mmap(NULL, sizeof(struct PTE) * pages_number, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (table == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    printf("-------------------------\n");
    printf("Initializing page table\n");
    for (int i = 0; i < pages_number; i++) {
        table[i].valid = false;
        table[i].frame = -1;
        table[i].dirty = false;
        table[i].referenced = 0;
        printf("Page %d---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, table[i].valid, table[i].frame, table[i].dirty, table[i].referenced);
    }
    printf("Create RAM\n");
    RAM = malloc(sizeof(char *) * frames);
    if (RAM == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    printf("Initialize RAM\n");
    for (int i = 0; i < frames; i++) {
        RAM[i] = malloc(sizeof(char) * SIZE);
        if (RAM[i] == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        RAM[i][0] = '\0';
    }

    printf("-------------------------\n");
    printf("Initialized RAM\nRAM array\n");
    for (int i = 0; i < frames; i++) {
        printf("Frame %d ---> %s\n", i, RAM[i]);
    }
    printf("-------------------------\n");
    printf("Initializing disk\n");
    disk = malloc(sizeof(char *) * pages_number);
    for (int i = 0; i < pages_number; i++) {
        disk[i] = malloc(sizeof(char) * SIZE);
        if (disk[i] == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
    }
    srand(time(NULL));
    for (int i = 0; i < pages_number; i++) {
        for (int j = 0; j < SIZE - 1; j++) {
            disk[i][j] = 'a' + rand() % 26;
        }
        disk[i][SIZE - 1] = '\0';
    }

    printf("Initialized disk\nDisk array\n");
    for (int i = 0; i < pages_number; i++) {
        printf("Page %d ----> %s\n", i, disk[i]);
    }

    signal(SIGUSR1, sigusr1_handler);
    while (1) {}

}

void sigusr1_handler() {
    int zero_referenced = 0;

    printf("MMU process (pid=%d) is running\n", table->mmu_pid);
    for (int i = 0; i < pages_number; i++) {
        if (table[i].referenced != 0) {
            if (current_frame < frames) {
                if (!table[i].valid) {
                    printf("A disk access request from MMU Process (pid=%d)\n", table->mmu_pid);
                    printf("Page %d is referenced\n", i);
                    table[i].referenced = 0;
                    table[i].frame = current_frame;
                    table[i].valid = 1;
                    printf("We can allocate it to free frame %d\n", table[i].frame);
                    printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, table[i].frame);
                    
                    RAM[table[i].frame][0] = '\0';
                    strcpy(RAM[current_frame], disk[i]);

                    printf("RAM array\n");
                    for (int k = 0; k < frames; k++) {
                        printf("Frame %d ---> %s\n", k, RAM[k]);
                    }

                    disk_accesses++;
                    printf("disk accesses is %d so far\n", disk_accesses);
                    current_frame++;
                    printf("Resume MMU process\n");
                    printf("-------------------------\n");
                    kill(table->mmu_pid, SIGCONT);
                }
            } else {
                printf("current_frame is %d\n", current_frame);
                printf("A disk access request from MMU Process (pid=%d)\n", table->mmu_pid);
                printf("Page %d is referenced\n", i);
                table[i].referenced = 0;
                printf("We do not have free frames in RAM\n");
                int victim_page = -1;
                while (1) {
                    int random_index = rand() % pages_number;
                    if (table[random_index].valid == 1) {
                        victim_page = random_index;
                        break;
                    }
                }
                printf("We choose page %d as a victim page\n", victim_page);
                printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, table[victim_page].frame);
                printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, table[victim_page].frame);
                RAM[table[victim_page].frame][0] = '\0';
                strcpy(RAM[table[victim_page].frame], disk[i]);

                printf("RAM array\n");
                for (int k = 0; k < frames; k++) {
                    printf("Frame %d ---> %s\n", k, RAM[k]);
                }

                if (table[victim_page].dirty == 1) {
                    printf("The victim page is dirty\n");
                    printf("Copy data from RAM (frame=%d) to disk (page=%d)\n", table[victim_page].frame, victim_page);
                    strcpy(disk[victim_page], RAM[table[victim_page].frame]);
                    printf("Disk array\n");
                    for (int k = 0; k < pages_number; k++) {
                        printf("Page %d ---> %s\n", k, disk[k]);
                    }
                    disk_accesses++;
                    printf("disk accesses is %d so far\n", disk_accesses);
                }

                int frame = table[victim_page].frame;
                table[victim_page].valid = 0;
                table[victim_page].frame = -1;
                table[victim_page].dirty = 0;
                table[victim_page].referenced = 0;
                table[i].frame = frame;
                table[i].valid = 1;
                table[i].dirty = 0;
                table[i].referenced = 0;
                printf("Resume MMU process\n");
                printf("-------------------------\n");
                kill(table->mmu_pid, SIGCONT);
            }
            disk_accesses++;
        } else {
            zero_referenced++;
        }
    }
    if (zero_referenced == pages_number) {
        printf("%d disk accesses in total\n", disk_accesses);
        printf("Pager is terminated\n");

        munmap(table, sizeof(struct PTE) * pages_number);

        exit(EXIT_SUCCESS);
    }
}