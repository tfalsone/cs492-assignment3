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
#include "GTree.hh"

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
        ifstream myfile;

        myfile.open(infoDir);
        //Add all the directories to the tree
        if (myfile.is_open())
        {
            while (!myfile.eof())
            {
                myfile >> text;
                myTree->add(text, -1, nullptr, "", -1, "");
            }
            myfile.close();
        }
        
        //Add all files to the tree
        myfile.open(infoFile);
        if (myfile.is_open())
        {
            while (!myfile.eof())
            {
                //Get size
                for(int x = 1; x < 7; x++)
                    myfile >> text;
                int fileSize = stoi(text);
                cout<< fileSize;
                
                //Get month
                myfile >> text;
                string month = text;
                cout<< month;
                
                //Get day
                myfile >> text;
                int day = stoi(text);
                cout<< day;
                
                //Get time
                myfile >>text;
                string tm = text;
                cout<< tm;
                
                //Get name
                myfile >> text;
                string fileName = text;
                cout<< fileName;
                
                
                //printf("%s: %d bytes\n", fileName.c_str(), fileSize);
                LFile *myFileList = new LFile(ceil((double)fileSize/blockSize), blockSize, ldisk, fileSize);
                
                myTree->add(fileName, fileSize, myFileList, month, day, tm);
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
