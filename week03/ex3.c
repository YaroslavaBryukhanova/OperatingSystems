#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Directory Directory;
typedef struct File File;

struct File{
    int id;
    char name[64];
    long unsigned int size;
    char data[1024];
    struct Directory * directory;
};

struct Directory{
    char name[64];
    struct File * files[256];
    struct Directory * directories[8];
    unsigned char nf;
    unsigned char nd;
    char path[2048];
};

void overwrite_to_file(struct File* file, const char* str);
void append_to_file(struct File* file, const char* str);
void printp_file(struct File* file);

void add_file(struct File* file, struct Directory* dir);
void show_file(struct File *file);
void show_dir(struct Directory *dir);
void add_dir(struct Directory *dir1, struct Directory *dir2);


int main() {

    Directory home, bin, root;
    
    home.nd = 0;
    home.nf = 0;
    strcpy(home.path, "/home");

    bin.nd = 0;
    bin.nf = 0;
    strcpy(bin.path, "/bin");

    root.nd = 0;
    root.nf = 0;
    strcpy(root.path, "/");

    add_dir(&home, &root);
    add_dir(&bin, &root);

    char content1[] = "int printf(const char * format, ...);";
    char content2[] = "//This is a comment in C language";
    char content3[] = "Bourne Again Shell!!";
    char content4[] = "int main(){printf(”Hello World!”)}";

    File ex3_1, bash, ex3_2;

    strcpy(ex3_1.name, "ex3_1.c");
    strcpy(ex3_2.name, "ex3_2.c");
    strcpy(bash.name, "bash");

    ex3_1.size = 0;
    ex3_2.size = 0;
    bash.size = 0;

    add_file(&bash, &bin);
    add_file(&ex3_1, &home);
    add_file(&ex3_2, &home);

    overwrite_to_file(&ex3_1, content1);
    overwrite_to_file(&ex3_2, content2);
    overwrite_to_file(&bash, content3);
    append_to_file(&ex3_1, content4);

    printp_file(&ex3_1);
    printp_file(&ex3_2);
    printp_file(&bash);

    return 0;
}

void add_file(struct File* file, struct Directory* dir){
    dir->files[dir->nf] = file;
    dir->nf++;
    file->directory = dir;
}

void overwrite_to_file(struct File* file, const char* str){
    strcpy(file->data, str);
    file->size = strlen(str);
}

void append_to_file(struct File* file, const char* str){
    strncat(file-> data, str, strlen(str));
    file->size = strlen(file->data);
}

void printp_file(struct File* file){
    printf("%s/%s \n", file->directory->path, file->name);
}

void show_file(File *file) {
    printf("%s", file->name);
}

void show_dir(Directory *dir) {
    printf("\nDIRECTORY\n");
    printf(" path: %s\n", dir->path);
    printf(" files:\n");
    printf(" [ ");
    for (int i = 0; i < dir->nf; i++) {
        show_file((dir->files[i]));
    }
    printf("]\n");
    printf(" directories:\n");
    printf(" { ");
    for (int i = 0; i < dir->nd; i++) {
        show_dir(dir->directories[i]);
    }
    printf("}\n");
}

void add_dir(Directory *dir1, Directory *dir2) {
    if (dir1 && dir2) {
        dir2->directories[dir2->nd] = dir1;
        dir2->nd++;
    }
}


