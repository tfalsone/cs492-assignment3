#include "filesystem.h"

void init_params(int argc, char *argv[]) {
    // put user arguments into global variables
    if (argc != 5) { printf("Incorrect number of arguments provided\n"); exit(1); }

    int size;
    // argv[1] --> input file for info on files
    INPUT_FILE = argv[1];

    // argv[2] --> input file for info on directories
    INPUT_DIR = argv[2];

    // argv[3] --> disk size
    DISK_SIZE = atoi(argv[3]);

    // argv[4] --> block size
    BLOCK_SIZE = atoi(argv[4]);
}

int main(int argc, char *argv[]) {
    // get input, initialize file system

}