#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <time.h>


#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

void print_directory(const char *path);
void print_event(struct inotify_event *event);
void signal_handler(int signal);
void stat_info(char *path);

int main(int argc, char *argv[]) {

    if (argc < 1) {
        exit(EXIT_FAILURE);
    }
    char *path = argv[1];

    print_directory(path);
    signal(SIGINT, signal_handler);

    int fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(fd, path, IN_ALL_EVENTS);
    if (wd == -1 ) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    char buffer[EVENT_BUF_LEN];
    while (1) {
        int len = read(fd, buffer, EVENT_BUF_LEN);
        // if (len == -1 && errno != EAGAIN) {
        //     perror("read");
        //     exit(EXIT_FAILURE);
        // }

        if (len > 0) {
            for (char *ptr = buffer; ptr < buffer + len; ptr += EVENT_SIZE + ((struct inotify_event *)ptr)->len) {
                struct inotify_event *event = (struct inotify_event *)ptr;
                if (event->len > 0) {
                    printf("File %s ", event->name);
                    if (event->mask & IN_CREATE) printf("created\n");
                    if (event->mask & IN_DELETE) printf("deleted\n");
                    if (event->mask & IN_MODIFY) printf("modified\n");
                }
            }
        }
    }
    close(fd);
}

void signal_handler(int signal) {
    printf("SIGINT\n");
    exit(EXIT_SUCCESS);
}

void print_event(struct inotify_event *event) {
    if(event -> mask & IN_ACCESS) {
        printf("File was accessed\n");
    }
    if(event -> mask & IN_CREATE) {
        printf("File/directory created in watched directory\n");
    }
    if(event -> mask & IN_DELETE) {
        printf("File/directory deleted from watched directory\n");
    }
    if(event -> mask & IN_MODIFY) {
        printf("File was modified\n");
    }
    if(event -> mask & IN_OPEN) {
        printf("File or directory was opened\n");
    }
    if(event -> mask & IN_ATTRIB) {
        printf("Metadata changed\n");
    }
}

void print_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            // Skip "." and ".." directories
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                continue;
            }
        } else {
            char *new_path = malloc(strlen(path) + strlen(entry -> d_name) + 2);
            strcpy(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, entry -> d_name);
            stat_info(new_path);
        }
    }
    closedir(dir);
}

void stat_info(char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        printf("Stat info for %s:\n", path);
        printf("  - Inode: %lu\n", st.st_ino);
        printf("  - Size: %ld bytes\n", st.st_size);
        printf("  - Mode: %o\n", st.st_mode);
        printf("  - UID: %d\n", st.st_uid);
        printf("  - GID: %d\n", st.st_gid);
    } else {
        perror("Error getting stat info");
    }
}

