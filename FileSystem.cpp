#include <stdio.h>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

class Block
{
public:
    bool free;
    Block *next;
    vector<int> ids;
    
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

//For each file in file_list.txt
class LFile
{
public:
    FileBlock *head;
    FileBlock *tail;
    int blockSize;
    int size;
    vector<int> iOwn;
    
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
    void addFileBlocks(vector<int> newOwn)
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
    vector<int> occupyBlocks(int numBlock, LDisk *disk, int byteSize)
    {
        vector<int> ret;
        
        if(this->iOwn.size() > 0) //check if any open memory in blocks this file has
        {
            for(int i = 0; i < iOwn.size(); i++)
            {
                if(disk->blockSpace[iOwn.at(i)] > 0)
                {
                    int sub = min(byteSize, disk->blockSpace[iOwn.at(i)]);
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
};

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
        this->next = NULL;
        this->myFiles = NULL;
        this->parent = NULL;
    }
    
    //Creates a node. This constructor is called for files
    Node(string name, int size, LFile *list, string month, int day, string ttime)
    {
        this->name = name;
        this->file = true;
        this->size = size;
        this->next = NULL;
        this->myFiles = list;
        this->parent = NULL;
        
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
            thisTime->tm_min = stoi(ttime.substr(ind + 1, 2));
            
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

int main(int argc, char *argv[])
{
    // ./FileSystem file_list.txt dir_list.txt 512 16
    if(argc == 5)
    {
        string infoFile = argv[1];
        string infoDir = argv[2];
        int diskSize = stoi(argv[3]);
        int blockSize = stoi(argv[4]);
        
        int totalBlocks = ceil((double)diskSize/blockSize);
        
        LDisk *ldisk = new LDisk(totalBlocks, blockSize);        //Blocks all connected in linked list
        
        ldisk->printBlocks();
        
        GTree *myTree = new GTree();
        
        string text;
        ifstream myfile(infoDir);
        //Add all the directories to the tree
        if (myfile.is_open())
        {
            while (myfile >> text)
            {
                myTree->add(text, -1, NULL, "", -1, "");
            }
            myfile.close();
        }
        
        //Add all files to the tree
        myfile.open(infoFile);
        if (myfile.is_open())
        {
            while (myfile >> text)
            {
                //Get size
                int fileSize;
                for(int x = 1; x < 7; x++)
                    myfile >> text;
                fileSize = stoi(text);
                
                //Get month
                string month;
                myfile >> text;
                month = text;
                
                //Get day
                int day;
                myfile >> text;
                day = stoi(text);
                
                //Get time
                string time;
                myfile >>text;
                time = text;
                
                //Get name
                string fileName;
                myfile >> text;
                fileName = text;
                
                
                //printf("%s: %d bytes\n", fileName.c_str(), fileSize);
                LFile *myFileList = new LFile(ceil((double)fileSize/blockSize), blockSize, ldisk, fileSize);
                
                myTree->add(fileName, fileSize, myFileList, month, day, time);
                //printf("%s: %d bytes\n", fileName.c_str(), fileSize);
            }
            myfile.close();
        }
        printf("--------------------------------------------------------------------\n");
        string command;
        string input;
        string input2;
        string line;
        
        //Runs until exit is called
        while(true)
        {
            //Take in the input
            getline(cin, line);
            
            int spaceI = line.find(' ', 0);
            command = line.substr(0, spaceI);
            
            //Based on the command, perform some operation
            if(command.compare("cd") == 0)
            {
                input = line.substr(spaceI + 1, line.length() - spaceI - 1);
                myTree->cd(input);
            }
            else if(command.compare("ls") == 0)
            {
                myTree->ls();
            }
            else if(command.compare("dir") == 0)
            {
                myTree->dir();
            }
            else if(command.compare("mkdir") == 0)
            {
                input = line.substr(spaceI + 1, line.length() - spaceI - 1);
                myTree->mkdir(input);
            }
            else if(command.compare("create") == 0)
            {
                input = line.substr(spaceI + 1, line.length() - spaceI - 1);
                myTree->create(input, blockSize);
            }
            else if(command.compare("append") == 0)
            {
                int nextInd = line.find(' ', spaceI + 1);
                input = line.substr(spaceI + 1, nextInd - spaceI - 1);
                input2 = line.substr(nextInd + 1, line.length() - nextInd - 1);
                myTree->append(input, stoi(input2), ldisk, blockSize);
            }
            else if(command.compare("remove") == 0)
            {
                int nextInd = line.find(' ', spaceI + 1);
                input = line.substr(spaceI + 1, nextInd - spaceI - 1);
                input2 = line.substr(nextInd + 1, line.length() - nextInd - 1);
                myTree->remove(input, stoi(input2));
            }
            else if(command.compare("delete") == 0)
            {
                input = line.substr(spaceI + 1, line.length() - spaceI - 1);
                myTree->deleteThing(input);
            }
            else if(command.compare("exit") == 0)
            {
                myTree->exitOut();
            }
            else if(command.compare("prfiles") == 0)
            {
                myTree->prfiles();
            }
            else if(command.compare("blocks") == 0)
            {
                ldisk->printBlocks();
                ldisk->printBlockSpace();
            }
            else if(command.compare("prdisk") == 0)
            {
                ldisk->prdisk();
            }
            else
            {
                printf("Command not recognized.\n");
            }
            printf("--------------------------------------------------------------------\n");
        }
    }
    else
    {
        printf("Argument list is incorrect. Use [file name storing information of files] [file storing information on directories] [disk size] [block size]\n");
    }
    
}
