#include "filesystem.h"

int getLastIndex(char* path, char c) {
    int i = strlen(path)-1;
    if (path != NULL) {
        while (i >= 0) {
            if (path[i--] == c) return ++i;
        }
    }
    return -1;
}

meta_info* init_file(char** vect) {
    char* name;
    meta_info* new;

    if (atoi(vect[6]) > freespace) {
        printf("Out of space\n");
    }

    new = (meta_info*)malloc(sizeof(meta_info)+1);

    name = vect[10];
    new->dir_fullname = (char*)malloc(sizeof(char)*strlen(name)+1);
    new->dir_fullname = name;
    new->dir_fullname[strlen(name)] = '\0';

    int i, j;
    i = getLastIndex(name, '/');
    
    new->dir_path = (char*)malloc(sizeof(char)*(strlen(name)*i+1));
    for (j = 0; j < i; j++)
        new->dir_path[j] = name[j];
    new->dir_path[j] = '\0';

    new->dir_name = (char*)malloc(sizeof(char)*(strlen(name))-i+1);
    for (j = ++i; j < strlen(name)+1; j++)
        new->dir_name[j-i] = name[j];
    new->dir_name[j-i] = '\0';

    new->file_size = atoi(vect[6]);
    new->tv = init_timestamp(vect[7], vect[8], vect[9]);
    new->LFile = create_Lfile(new->file_size);
}

int get_fragmentation(int filesize) {
    int i = filesize;
    if (i == 0) return 0;
    int rem = i % BLOCK_SIZE;
    return BLOCK_SIZE - rem;
}

void add_file_to_space(int filesize) {
    freespace -= filesize;
    fragmentation += get_fragmentation(filesize);
}