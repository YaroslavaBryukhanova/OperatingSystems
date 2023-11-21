#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    // Open the root directory
    DIR *dir = opendir("/");

    // Check if the directory opening was successful
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Read the directory entries
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);

    return 0;
}
