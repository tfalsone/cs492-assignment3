#include "filesystem.h"

// Initialize Ldisk (represents disk space)
void init_LDisk() {
    BLOCK_TOTAL = DISK_SIZE / BLOCK_SIZE;

    Ldisk = (disk*)malloc(sizeof(disk));
    Ldisk->status = FREE;
    Ldisk->begin = 0;
    Ldisk->end = BLOCK_TOTAL-1;
    Ldisk->next = NULL;

    fragmentation = 0;
    freespace = BLOCK_TOTAL * BLOCK_SIZE;    
}

// Initialize directory
dir* init_dir(dir* parent, char* name) {
    dir *newDir;
    newDir = (dir*)malloc(sizeof(dir)+1);
    newDir->parentDir = parent;

    newDir->dir_fullname = (char*)malloc(sizeof(char)*(strlen(name)+2));
    strcpy(newDir->dir_fullname, name);
    newDir->dir_fullname[strlen(name)] = '\0';
    if (parent != NULL) {
        newDir->dir_fullname = strcat(newDir->dir_fullname, "/");
    } 
    
    // newDir->dir_name = (char*)malloc(sizeof(char)*(strlen(name)))

    newDir->GDir = (queue*)malloc(sizeof(queue)+1);
    q_init(newDir->GDir);
    newDir->GFiles = (queue*)malloc(sizeof(queue)+1);
    q_init(newDir->GFiles);
}

// Initialize directed tree structure (G)
void init_GDirs() {
    dir *mydir, *new;

    FILE *fd;
    char ch[MAX];
    char c;
    int t = 0;

    fd = fopen(INPUT_DIR, "r");
    fflush(fd);
    if(fd == NULL) {
        printf("Cannot open input directory file");
        exit(1);
    }

    while((c = fgetc(fd)) != EOF) {
        // Iterate through directories list and initialize all directories
        do {
            ch[t++] = c;
        } while ((c = fgetc(fd)) != '\n');
        ch[t++] = '\0';

        if (root == NULL) {
            // setup root directory (should be first line in directories file)
            root = init_dir(root, ch);
            dir_count++;
        } else {
            mydir = find_dir(root, ch);
            if (strcmp(mydir->dir_fullname, ch)) {
                new = init_dir(mydir, ch);
                q_push(mydir, new, 0);
                dir_count++;
            }
        }

        t = 0;
    }
    fflush(fd);
    fclose(fd);
}
// TODO - finish
void init_GFiles() {
    meta_info *new;
    dir *curr;

    FILE *fd;
    char ch[MAX];
    char c;
    int t;
    char** vect;

    fd = fopen(INPUT_FILE, "r");
    fflush(fd);
    if (fd == NULL) {
        printf("Unable to open input file\n");
        exit(1);
    }

    while((c = fgetc(fd)) != EOF) {
        t = 0;
        do {
            ch[t++] = c;
        } while ((c = fgetc(fd)) != '\n');
        ch[t++] = '\0';


    }
}

file* create_Lfile(int file_size) {
    file *Lfile, *current, *new;
    int blocks;

    if (file_size > 0) {
        blocks = ceil(file_size / BLOCK_SIZE);
    } else return (file*)NULL;

    for (int i = 0; i < blocks; i++) {
        new = (file*)malloc(sizeof(file));
        new->addr = getLdisk() * BLOCK_SIZE;
        new->next = NULL;
        if (i == 0) {
            Lfile = new;
            current = new;
        } else {
            current->next = new;
            current = current->next;
        }
    }
}