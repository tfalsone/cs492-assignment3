#pragma once

#include <stdio.h>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>
#include <iostream>

#include "Block.hh"
#include "LDisk.hh"
#include "FileBlock.hh"

//For each file in file_list.txt
class LFile
{
public:
    FileBlock *head;
    FileBlock *tail;
    int blockSize;
    int size;
    std::vector<int> iOwn;
    
    //Creates the Lfile
    LFile(int numBlock, int blockSize, LDisk* disk, int byteSize)
    {
        this->head = NULL;
        this->tail = NULL;
        
        this->blockSize = blockSize;
        if(numBlock > 0)
        {
            iOwn = this->occupyBlocks(numBlock, disk, byteSize);
            this->addFileBlocks(iOwn);
        }
    }
    
    //Adds to the file blocks
    void addFileBlocks(std::vector<int> newOwn)
    {
        int size = newOwn.size();
        if(this->size > 0)
        {
            FileBlock *elem = new FileBlock(newOwn.at(0), blockSize);
            iOwn.push_back(newOwn.at(0));
            if(head == NULL)
            {
                this->head = elem;
                this->tail = elem;
            }

            for(int i = 1; i < size; i++)
            {
                elem = new FileBlock(newOwn.at(i), blockSize);
                iOwn.push_back(newOwn.at(1));
                this->tail->next = elem;
                this->tail = elem;
            }
        }
    }
    
    //Take up blocks in LDisk
    std::vector<int> occupyBlocks(int numBlock, LDisk *disk, int byteSize)
    {
        std::vector<int> ret;
        
        if(this->iOwn.size() > 0) //check if any open memory in blocks this file has
        {
            for(int i = 0; i < iOwn.size(); i++)
            {
                if(disk->blockSpace[iOwn.at(i)] > 0)
                {
                    int sub = std::min(byteSize, disk->blockSpace[iOwn.at(i)]);
                    disk->blockSpace[iOwn.at(i)] -= sub;
                    byteSize -= sub;
                    if(byteSize == 0)
                        return ret;
                }
            }
        }
        
        int length = disk->size;
        Block *curr =  disk->head;
        
        for(int i = 0; i < length; i++)
        {
            if(curr->free) //this node is free so look through it
            {
                for(int j = 0; j < curr->ids.size(); j++) //look through the ids in this node
                {
                    ret.push_back(curr->ids.at(j));
                    numBlock--;
                    if(numBlock == 0)
                    {
                        disk->takeBlocks(ret, byteSize);
                        return ret;
                    }
                }
            }
            curr = curr->next;
        }
        printf("Not enough space for the file.\n");
        ret.clear();
        return ret;
    }
    
    //Adds a file block
    void add(FileBlock *elem)
    {
        if(head == NULL)
        {
            head = elem;
            tail = elem;
        }
        else
        {
            tail->next = elem;
            tail = elem;
        }
    }

    void printLFile() {
        printf("test\n");
    }
};