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

using namespace std;

class Node
{
public:
    string name;
    bool file;          //true when file, false when directory
    int size;           //size of file
    LFile *myFiles;     //linked-list-data structure storing the disk block addresses of the file
    Node *next;         //Next node
    vector<Node*> myNodes; //All children nodes
    Node *parent;       //parent node
    struct tm *theTime; //the time
    
    //Creates a node. This constructor is used for directories
    Node(string name)
    {
        this->name = name;
        this->file = false;
        this->size = -1;
        this->next = nullptr;
        this->myFiles = nullptr;
        this->parent = nullptr;
    }
    
    //Creates a node. This constructor is called for files
    Node(string name, int size, LFile *list, string month, int day, string ttime)
    {
        this->name = name;
        this->file = true;
        this->size = size;
        this->next = nullptr;
        this->myFiles = list;
        this->parent = nullptr;
        
        struct tm *thisTime;
        
        if(day > 0) //day == -1 when a file in created during run time through create
        {
            int mon;
            if(month.compare("Jan") == 0)
            {
                mon = 0;
            }
            else if(month.compare("Feb") == 0)
            {
                mon = 1;
            }
            else if(month.compare("Mar") == 0)
            {
                mon = 2;
            }
            else if(month.compare("Apr") == 0)
            {
                mon = 3;
            }
            else if(month.compare("May") == 0)
            {
                mon = 4;
            }
            else if(month.compare("Jun") == 0)
            {
                mon = 5;
            }
            else if(month.compare("Jul") == 0)
            {
                mon = 6;
            }
            else if(month.compare("Aug") == 0)
            {
                mon = 7;
            }
            else if(month.compare("Sep") == 0)
            {
                mon = 8;
            }
            else if(month.compare("Oct") == 0)
            {
                mon = 9;
            }
            else if(month.compare("Nov") == 0)
            {
                mon = 10;
            }
            else if(month.compare("Dec") == 0)
            {
                mon = 11;
            }
            else
            {
                printf("Month input is wrong: %s\n", month.c_str());
                mon = 12;
            }
            
            thisTime->tm_mon = mon;
            thisTime->tm_mday = day;
            int ind = ttime.find(':', 0);
            thisTime->tm_hour = stoi(ttime.substr(0, ind));
            thisTime->tm_min = stoi(ttime.substr(ind + 1, ttime.size()));
            
            theTime = thisTime;
        }
        else
        {
            time_t timer;
            time_t t = time(&timer);
            theTime = gmtime(&t);
        }
    }
    
    //Updates time to current time
    void updateTime()
    {
        time_t timer;
        time_t t = time(&timer);
        theTime = gmtime(&t);
    }
    
    void add(Node* node)
    {
        this->myNodes.push_back(node);
    }
    
    //Gets the time in the form of a string
    string getTime()
    {
        string hour = to_string(theTime->tm_hour);
        string min = to_string(theTime->tm_min);
        string month = to_string(theTime->tm_mon);
        string day = to_string(theTime->tm_mday);
        string ret =  "" + hour + ":" + min + " - " + month + "/" + day;
        
        return ret;
    }
    
};
