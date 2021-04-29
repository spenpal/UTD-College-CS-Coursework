// Project:      Project 2
// Filename:     mod-v6.cpp
// Date:         4/29/21
// Author:       Sanjeev Penupala, Gautam Sapre
// Course:       CS 4348.006
// 
// Description:
// 
//      Implement V6 file system
//
// ----------------------------------------------------------------

// IMPORTS //
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

// GLOBALS //
int fd;

// STRUCTS //
typedef struct 
{
    int isize;
    int fsize;
    int nfree;
    unsigned int free[250];
    unsigned int ninode;
    unsigned int inode[250];
    int flock;
    int ilock;
    unsigned intfmod;
    unsigned inttime;
} superblock_type; // Blocksize is 2048 Bytes

superblock_type superBlock;

typedef struct 
{
    unsigned shortflags;
    unsigned intnlinks;
    unsigned intuid;
    unsigned intgid;
    unsigned int size;
    unsigned int addr[9];
    unsigned short actime[2];
    unsigned short modtime[2];
    unsigned short dummy; // not used
} inode_type; // 64 Bytes in size

typedef struct 
{
    unsigned int inode;
    unsigned char filename[28];
} dir_type;

enum commands
{
    QUIT,
    HELP,
    OPENFS,
    INITFS,
    COUNTFREE
};

// FUNCTIONS //

commands hashC(string command)
{
    if (command == "quit") return QUIT;
    if (command == "help") return HELP;
    if (command == "openfs") return OPENFS;
    if (command == "initfs") return INITFS;
    if (command == "count-free") return COUNTFREE;
}

vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);

   while (getline(tokenStream, token, delimiter))
      tokens.push_back(token);

   return tokens;
}

void help()
{
    ifstream f("help.txt");
    if (f.is_open())
        cout << f.rdbuf();
    f.close();
}

int openfs(string filename)
{
    fd = open(filename.c_str(), 2);
    return 1;
}

void initfs()
{
    return;
}

void countFree()
{
    return;
}

// DRIVER CODE //
int main()
{
    // Display Help Screen For User
    bool control = true;
    string store = "";
    ifstream f("help.txt");
    if (f.is_open())
        cout << f.rdbuf();
    f.close();

    // Accept commands from user until they quit
    while (control) 
    {
        cout << "\n" << endl;
        cout << "> ";
        cin >> store;

        vector<string> input = split(store, ' ');
        string command = input[0];
        transform(command.begin(), command.end(), command.begin(), ::tolower); // lowercase the command

        switch(hashC(command))
        {
            case QUIT: control = false; break;
            case HELP: help(); break;
            case OPENFS: openfs(input[1]); break;
            case INITFS: initfs(); break;
            case COUNTFREE: countFree(); break;
        }
    }
    
    return 0;
}