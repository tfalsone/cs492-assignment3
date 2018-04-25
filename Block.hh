#pragma once
#include <stdio.h>
#include <vector>

class Block
{
public:
    bool free;
    Block *next;
    std::vector<int> ids;
    
    //Creates an empty block
    Block()
    {
        free = true;
        this->next = NULL;
    }
    
    //Creates a block with an initail id
    Block(int id)
    {
        this->ids.push_back(id);
        free = true;
        this->next = NULL;
    }
    
    //Adds an id to the block
    void add(int id)
    {
        this->ids.push_back(id);
    }
    
    //Prints the individual block
    void printBlock()
    {
        printf("Block ");
        for(int i = 0; i < ids.size(); i++)
        {
            printf("%i ", ids.at(i));
        }
        printf("%s\n", this->free ? "true" : "false");
    }
    
};