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
#include "LFile.hh"
#include "Node.hh"

class GTree
{
public:
    Node *parent;
    Node *FINALROOT;
    Node *root;
    Node *newLoc;
    
    GTree()
    {
        root = new Node("./");
        FINALROOT = root;
        parent = NULL;
    }
    
    //Add node to the tree
    void add(string name, int size, LFile *list, string month, int day, string time)
    {
        int i, j;
        Node *curr = root;
        Node *elem;
        
        string currS;
        bool hit = false;
        for(i = 2, j = 2; i < name.length(); i++)
        {
            if(name.at(i) == '/')//go down tree
            {
                if(hit)
                {
                    currS = name.substr(j + 1, i - j - 1);
                    int len;
                    int size = curr->myNodes.size();
                    for(len = 0; len < size; len++)
                    {
                        if(curr->myNodes.at(len)->name.compare(currS) == 0)
                        {
                            curr = curr->myNodes.at(len);
                            break;
                        }
                    }
                    if(len == size)
                        printf("Something went wrong\n");
                    j = i;
                }
                else
                    hit = true;
                
            }
            
        }
        if(size != -1 || i != 2) //size == -1 when directory
        {
            if(size != -1) //is a file
            {
                elem = new Node(name.substr(j + 1, i - j), size, list, month, day, time);
                elem->parent = curr;
                curr->add(elem);
            }
            else if(i != 2) //not the root entry
            {
                elem = new Node(name.substr(j + 1, i - j));
                elem->parent = curr;
                curr->add(elem);
            }
        }
    }
    
    //Puts the node newLoc at the directory structure of name. Used when input looks like
    // mkdir A/B/W allowing the after function to begin work in the B directory
    string getMeThere(string name)
    {
        int i, j;
        string part;
        bool firstPass = false;
        newLoc = root;
        for(i = 0, j = 0; i < name.length(); i++)
        {
            //Looks for the '/'
            if(name.at(i) == '/')
            {
                if(firstPass)
                    part = name.substr(j + 1, i - j - 1);
                else
                    part = name.substr(j, i);
                
                firstPass = true;
                int k;
                for(k = 0; k < newLoc->myNodes.size(); k++)
                {
                    if(newLoc->myNodes.at(k)->name.compare(part) == 0)
                    {
                        newLoc = newLoc->myNodes.at(k);
                        break;
                    }
                }
                if(k == newLoc->myNodes.size())
                {
                    printf("Could not find that directory.\n");
                    return "";
                }
                j = i;
            }
        }
        
        if(firstPass)
            part = name.substr(j + 1, i - j - 1);
        else
            part = name.substr(j, i);
        
        //Returns the last part of the name
        return part;
    }
    
    //Change directory
    void cd(string name)
    {
        if(name.compare("..") == 0)
        {
            if(this->parent != NULL)
            {
                this->root = this->parent;
                this->parent = this->parent->parent;
            }
            else
            {
                printf("Already in root directory.\n");
            }
        }
        else
        {
            string part = this->getMeThere(name);
            if(part.compare("") == 0)
                return;
            
            int k;
            for(k = 0; k < newLoc->myNodes.size(); k++)
            {
                if(newLoc->myNodes.at(k)->name.compare(part) == 0)
                {
                    this->parent = newLoc;
                    newLoc = newLoc->myNodes.at(k);
                    this->root = newLoc;
                    return;
                }
            }
            printf("Could not find that directory.\n");
        }
        
    }
    
    //List all files and directories in current directory
    void ls()
    {
        for(int i = 0; i < this->root->myNodes.size(); i++)
            printf("%s\n", this->root->myNodes.at(i)->name.c_str());
    }
    
    //Print tree from current root node
    void dir()
    {
        //Prints tree using breadth-first
        vector<Node*> toPrint;
        toPrint.push_back(root);
        while(toPrint.size() > 0)
        {
            //push all children on to the queue
            for(int i = 0; i < toPrint.at(0)->myNodes.size(); i++)
                toPrint.push_back(toPrint.at(0)->myNodes.at(i));
            
            printf("%s\n", toPrint.at(0)->name.c_str());
            toPrint.erase(toPrint.begin());
        }
    }
    
    //Print files and their attributes under current directory
    void prfiles()
    {
        //Prints tree using breadth-first
        vector<Node*> toPrint;
        toPrint.push_back(root);
        Node *curr = root;
        while(toPrint.size() > 0)
        {
            //push all children on to the queue
            for(int i = 0; i < toPrint.at(0)->myNodes.size(); i++)
                toPrint.push_back(toPrint.at(0)->myNodes.at(i));
            
            if(toPrint.at(0)->size != -1) //is a file
            {
                printf("%s - %i bytes - %s\n", toPrint.at(0)->name.c_str(), toPrint.at(0)->size, toPrint.at(0)->getTime().c_str());
                toPrint.at(0)->myFiles->printLFile();
                
            }
            toPrint.erase(toPrint.begin());
        }
    }
    
    //Makes directory
    void mkdir(string name)
    {
        //mkdir CS492_HW2/VMSimulator.dSYM/newDirectory
        string part = this->getMeThere(name);
        if(part.compare("") == 0)
            return;
        
        int k;
        for(k = 0; k < newLoc->myNodes.size(); k++)
        {
            if(newLoc->myNodes.at(k)->name.compare(part) == 0)
            {
                printf("The name %s has already been used.\n", name.c_str());
                return;
            }
        }
        Node *newNode = new Node(part);
        newLoc->add(newNode);
    }
    
    //Create a file
    void create(string name, int blockSize)
    {
        string part = this->getMeThere(name);
        if(part.compare("") == 0)
            return;
        
        for(int i = 0; i < this->newLoc->myNodes.size(); i++)
        {
            if(this->newLoc->myNodes.at(i)->name.compare(part) == 0)
            {
                printf("The name %s has already been used.\n", name.c_str());
                return;
            }
        }
        string inp = "";
        Node *newNode = new Node(name, 0, new LFile(0, blockSize, NULL, 0), inp, -1, inp); //NULL LDisk bc the file has no size
        this->newLoc->add(newNode);
    }
    
    //Appends a number of bytes to the end of a file
    void append(string name, int bytes, LDisk *disk, int blockSize)
    {
        string part = this->getMeThere(name);
        if(part.compare("") == 0)
            return;
        
        for(int i = 0; i < this->newLoc->myNodes.size(); i++)
        {
            if(this->newLoc->myNodes.at(i)->name.compare(part) == 0)
            {
                this->newLoc->myNodes.at(i)->size += bytes;
                vector<int> newFileBlocks = this->newLoc->myNodes.at(i)->myFiles->occupyBlocks(ceil((double)bytes/blockSize), disk, bytes);
                this->newLoc->myNodes.at(i)->myFiles->addFileBlocks(newFileBlocks);
                return;
            }
        }
        printf("A file with that name is not in this directory.\n");
    }
    
    //Romve a number of bytes from a file
    void remove(string name, int bytes)
    {
        string part = this->getMeThere(name);
        if(part.compare("") == 0)
            return;
        
        for(int i = 0; i < this->newLoc->myNodes.size(); i++)
        {
            if(this->newLoc->myNodes.at(i)->name.compare(part) == 0)
            {
                this->newLoc->myNodes.at(i)->size -= bytes;
                return;
            }
        }
        printf("A file with that name is not in this directory.\n");
    }
    
    //Deletes directory (if empty) or file
    void deleteThing(string name)
    {
        string part = this->getMeThere(name);
        if(part.compare("") == 0)
            return;
        
        int k;
        for(k = 0; k < newLoc->myNodes.size(); k++)
        {
            if(newLoc->myNodes.at(k)->name.compare(part) == 0)
            {
                
                if(newLoc->myNodes.at(k)->myNodes.size() > 0) //directory with structures within
                {
                    printf("Directory is not empty.\n");
                }
                else
                {
                    newLoc->myNodes.erase(newLoc->myNodes.begin() + k);
                }
                return;
            }
        }
        printf("This name can not be found.\n");
    }
    
    void exitOut()
    {
        //de-allocate everything
        exit(1);
    }
    
    void printTree()
    {
        //Prints tree using breadth-first
        vector<Node*> toPrint;
        toPrint.push_back(FINALROOT);
        while(toPrint.size() > 0)
        {
            //push all children on to the queue
            for(int i = 0; i < toPrint.at(0)->myNodes.size(); i++)
                toPrint.push_back(toPrint.at(0)->myNodes.at(i));
            
            printf("%s\n", toPrint.at(0)->name.c_str());
            toPrint.erase(toPrint.begin());
        }
    }
    
};
