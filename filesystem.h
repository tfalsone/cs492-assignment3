#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

// Libraries
    #include <sys/time.h>
	#include <time.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/ioctl.h>
	#include <fcntl.h>
    #include <errno.h>
    #include "queue/queue_struct.h"

// Defines
    #ifndef NULL
        #define NULL ((void*) 0)
    #endif

    #define FALSE 0
    #define TRUE 1
    #define FREE 0
    #define USED 1

// Structures
    typedef int bool;
    struct timeval tvbegin;

    typedef struct directory {
        struct directory *parentDir;
        char* dir_name;
        char* dir_fullname;
        char* dir_path;
        queue* GDir;
        queue* GFiles;
    } dir;
    dir *root;

    typedef struct disk {
        bool status;
        int begin;
        int end;
        struct disk *next;
    } disk;

    typedef struct file {
        int addr;
        struct file *next;
    } file;

    typedef struct meta_info {
        char* dir_name;
        char* dir_fullname;
        char* dir_path;
        int file_size;
        time_t tv;
        file *LFile;
    } meta_info;

// Global variables
    char *INPUT_FILE;
    char *INPUT_DIR;
    unsigned long int DISK_SIZE;
    int BLOCK_SIZE;
    unsigned long int BLOCK_TOTAL;
    disk *Ldisk;

    int file_count;
    int dir_count;
    int fragmentation;
    int freespace;

// Functions
    // main TODO - add function definitions for various files


#endif