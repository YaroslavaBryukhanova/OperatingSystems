#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <limits.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))
#define MAX_PATH 1024

char *path;

void find_all_hlinks(const char* source);
void unlink_all(const char* source);
void create_sym_link(const char* source, const char* link);

int main(int argc, char *argv[]) {
    if (argc < 1) {
        exit(EXIT_FAILURE);
    }
    path = argv[1];

    char buffer[EVENT_BUF_LEN];
    char *file_path = malloc(MAX_PATH);

    strcpy(file_path, path);
    strcat(file_path, "/myfile1.txt");
    
    FILE* file = fopen(file_path, "w");

    fprintf(file, "Hello world.\n");
    fclose(file);

    printf("File created.\n");
    printf("%s\n", path);

    char* myfile11 = malloc(MAX_PATH);
    char* myfile12 = malloc(MAX_PATH);

    strcpy(myfile11, path);
    strcat(myfile11, "/myfile11.txt");

    if (link(file_path, myfile11) == -1) {
        perror("link");
        exit(1);
    }

    printf("Hard link created.\n");

    strcpy(myfile12, path);
    strcat(myfile12, "/myfile12.txt");
    
    if (link(file_path, myfile12) == -1) {
        perror("link");
        exit(1);
    }


    find_all_hlinks("myfile1.txt");

    char* myfile1_moved = malloc(MAX_PATH);
    strcpy(myfile1_moved, "/tmp/myfile1.txt");
    
    if (rename(file_path, myfile1_moved) == -1) {
        perror("rename");
        exit(EXIT_FAILURE);
    }

    printf("File moved.\n");

    file = fopen(myfile11, "a");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Modified.\n");
    fclose(file);

    printf("File modified.\n");

    char* myfile13 = malloc(MAX_PATH);
    strcpy(myfile13, path);
    strcat(myfile13, "/myfile13.txt");

    create_sym_link(myfile1_moved, myfile13);

    printf("Symbolic link created.\n");

    file = fopen(myfile1_moved, "a");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Modified.\n");
    fclose(file);

    printf("File modified.\n");

    unlink_all("myfile11.txt");

    printf("Hard links removed.\n");

    find_all_hlinks("myfile11.txt");

    free(file);
    free(myfile11);
    free(myfile12);
    free(myfile13);
    free(myfile1_moved);

    // close(fd);
    exit(EXIT_SUCCESS);  
    return 0;
}

void find_all_hlinks(const char* source) {
    DIR * directory;
    struct dirent *entry;
    struct stat statbuf;
    char fullpath[MAX_PATH];
    char realpathbuf[MAX_PATH];
    ino_t inode = -1;
    int count = 0;

    printf("source: %s\n", source);
    printf("path: %s\n", path);

    if ((directory = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    stat(strcat(strcat(strcpy(fullpath, path), "/"), source), &statbuf);
    if (stat(fullpath, &statbuf) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    inode = statbuf.st_ino;

    printf("inode: %ld\n", inode);
    printf("fullpath: %s\n\n", fullpath);

    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, MAX_PATH, "%s/%s", path, entry->d_name);

        if (stat(fullpath, &statbuf) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (S_ISREG(statbuf.st_mode)) {
            if (statbuf.st_ino == inode && strcmp(entry->d_name, source) != 0) {
                count++;
                realpath(fullpath, realpathbuf);
                printf("inode: %ld\t%s\n", statbuf.st_ino, realpathbuf);
            }
        }
    }

    closedir(directory);

    if (count == 0) {
        printf("No hard links found.\n");
    } else {
        printf("Total %d hard links found.\n", count);
    }
}

void unlink_all(const char* source) {
    DIR *directory;
    struct dirent* entry;
    struct stat statbuf;
    char fullpath[MAX_PATH];
    ino_t inode = -1;
    int count;

    if ((directory = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    stat(strcat(strcat(strcpy(fullpath, path), "/"), source), &statbuf);
    inode = statbuf.st_ino;

    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        strcpy(fullpath, path);
        strcat(fullpath, "/");
        strcat(fullpath, entry->d_name);

        if (stat(fullpath, &statbuf) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (S_ISREG(statbuf.st_mode)) {
            if (statbuf.st_ino == inode && strcmp(entry->d_name, source) != 0) {
                count++;
                if (unlink(fullpath) == -1) {
                    perror("unlink");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    closedir(directory);

    if (count == 0) {
        printf("No duplicate hard links found.\n");
    } else {
        printf("Removed %d duplicate hard links.\n", count);
    }
}

void create_sym_link(const char* source, const char* link) {
    if(symlink(source, link) == -1) {
        perror("symlink");
        exit(EXIT_FAILURE);
    }
}