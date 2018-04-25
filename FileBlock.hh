#include <stdio.h>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>
#include <iostream>

#include "Block.hh"
#include "LDisk.hh"

class FileBlock
{
public:
    int startAddr;
    int id;
    FileBlock *next;
    
    FileBlock(int id, int blockSize)
    {
        this->id = id;
        this->startAddr = id * blockSize;
    }
};