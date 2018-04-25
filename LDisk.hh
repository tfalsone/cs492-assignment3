#include <stdio.h>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>
#include <iostream>

#include "Block.hh"

class LDisk
{
public:
    int size;
    Block *head;
    Block *tail;
    
    int blockSize;
    int arrSize;
    int *blockSpace;
    
    LDisk(int size, int blockSize)
    {
        this->size = size;
        this->blockSpace = new int[size];
        this->arrSize = size;
        this->blockSize = blockSize;
        
        //Initialize all blocks
        this->blockSpace[0] = blockSize;
        Block *elem = new Block(0);
        head = elem;
        tail = elem;
        for(int i = 1; i < size; i++)
        {
            this->blockSpace[i] = blockSize;
            elem = new Block(i);
            tail->next = elem;
            tail = elem;
        }
        this->merge();
    }
    
    //Merges the block nodes
    void merge()
    {
        Block *prev = head;
        bool prevType = prev->free;
        Block *curr = head->next;
        bool currType = curr->free;
        int newSize = 1;
        
        for(int i = 1; curr != NULL; i++)
        {
            if(prevType == currType)
            {
                int idsSize = curr->ids.size();
                for(int j = 0; j < idsSize; j++)
                    prev->add(curr->ids.at(j));
                prev->next = curr->next;
                Block *temp = curr->next;
                delete curr;
                curr = NULL;
                curr = temp;
                if(curr != NULL)
                    currType = curr->free;
            }
            else
            {
                prev = curr;
                prevType = currType;
                
                curr = curr->next;
                if(curr != NULL)
                    currType = curr->free;
                
                newSize++;
            }
        }
        
        if(prev->next == NULL)
            tail = prev;
        
        this->size = newSize;
    }
    
    //Takes the blocks for the files
    void takeBlocks(vector<int> blockIds, int byteSize)
    {
        Block *prev = head;
        Block *curr = head;
        int ind = 0;
        int amount = blockIds.size();
        bool newHead = false;
        Block *newBlock;
        
        int totalInd = 0;
        
        for(int i = 0; curr != NULL && ind < amount; i++, totalInd++)
        {
            if(curr->free) //this node is free so look through it
            {
                Block *newBlock = new Block();
                newBlock->free = false; //is now taken
                
                for(int j = 0; j < curr->ids.size() && ind < amount; j++, totalInd++) //find the ids
                {
                    if(curr->ids.at(j) == blockIds.at(ind))
                    {
                        newBlock->add(curr->ids.at(j)); //create new block with id
                        curr->ids.erase(curr->ids.begin()+j); //erase from the current block
                        
                        int sub = min(this->blockSize, byteSize);
                        this->blockSpace[totalInd] -= sub;
                        byteSize -= sub;
                        
                        if(curr->ids.size() == 0) //just emptied the block and it will be deleted
                        {
                            newBlock->next = curr->next;
                        }
                        else
                        {
                            newBlock->next = curr;
                        }
                        
                        if(i == 0 && j == 0)//at the head and first id
                        {
                            newHead = true;
                        }
                        else//not at the head
                        {
                            //nothing to do
                        }
                        
                        ind++;
                        j = -1; //next iteration makes j == 0
                    }
                }
                
                //The following need to wait until the newBlock is done being created
                if(newHead)
                {
                    this->head = newBlock;
                }
                else
                {
                    prev->next = newBlock;
                }
                if(curr->ids.size() == 0) //just emptied the block
                {
                    Block *theNext = curr->next;
                    delete curr;
                    curr = NULL;
                    curr = theNext;
                    prev = curr;
                }
                else
                {
                    prev = curr;
                    curr = curr->next;
                }
            }
            else
            {
                
                totalInd += curr->ids.size() - 1;
                prev = curr;
                curr = curr->next;
            }
        }
        
        this->merge();
        //this->printBlocks();
    }
    
    //prints the blocks (used for tracing)
    void printBlocks()
    {
        Block *curr = head;
        for(int i = 0; i < size; i++)
        {
            curr->printBlock();
            curr = curr->next;
        }
    }
    
    //Prints the block space array (used for tracing)
    void printBlockSpace()
    {
        printf("[");
        for(int i = 0; i < this->arrSize; i++)
        {
            printf(" %i", this->blockSpace[i]);
        }
        printf("]\n");
    }
    
    //The method for the prdick command
    void prdisk()
    {
        bool inUse = blockSpace[0] != blockSize;
        int frag = 0;
        int start = 0;
        int end = 0;
        if(inUse && blockSpace[0] != 0)
            frag += blockSpace[0];
        int i;
        for(i = 1; i < this->arrSize; i++)
        {
            if(this->blockSpace[i] != blockSize && inUse)
            {
                end++;
                frag += blockSpace[i];
            }
            else if(this->blockSpace[i] == blockSize && !inUse)
                end++;
            else
            {
                printf("%s: %i-%i\n", inUse ? "In use" : "Free", start, end);
                start = ++end;
                inUse = !inUse;
            }
        }
        
        printf("%s: %i-%i\n", inUse ? "In use" : "Free", start, end);
        printf("fragmentation: %i bytes\n", frag);
    }
};
