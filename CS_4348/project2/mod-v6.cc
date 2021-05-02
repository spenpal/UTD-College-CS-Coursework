// Project:      Project 2
// Filename:     mod-v6.cc
// Date:         5/1/21
// Author:       Sanjeev Penupala, Gautam Sapre
// Course:       CS 4348.006
// 
// Description:
// 
//      Implement V6 file system
//
// ----------------------------------------------------------------

// IMPORTS & GLOBALS //
#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#define BLOCK_SIZE 2048
#define INODE_SIZE 64
#define DIR_SIZE 32
#define FREE_ARRAY_SIZE 250

using namespace std;

int fd = -1;

// STRUCTS //
typedef struct 
{
    int isize;
    int fsize;
    int nfree;
    unsigned int free[FREE_ARRAY_SIZE];
    unsigned int ninode;
    unsigned int inode[FREE_ARRAY_SIZE];
    int flock;
    int ilock;
    unsigned int fmod;
    unsigned int time;
} superblock_type; // Blocksize is 2048 Bytes
superblock_type superBlock;

typedef struct
{
    unsigned short flags;
    unsigned int nlinks;
    unsigned int uid;
    unsigned int gid;
    unsigned int size;
    unsigned int addr[9];
    unsigned short actime[2];
    unsigned short modtime[2];
    unsigned short dummy; // not used
} inode_type; // 64 Bytes in size
static inode_type root_inode;

typedef struct 
{
    unsigned int inode;
    unsigned char filename[28];
} dir_type; // 32 Bytes long
dir_type root_dir[2];

enum commands
{
    QUIT,
    HELP,
    OPENFS,
    INITFS,
    COUNTFREE,
    OTHER
};

// HELPER FUNCTIONS //
/**
 * Maps a command to its enum counterpart
 */
commands hashC(string command)
{
    if (command == "quit") return QUIT;
    if (command == "help") return HELP;
    if (command == "openfs") return OPENFS;
    if (command == "initfs") return INITFS;
    if (command == "count-free") return COUNTFREE;
    return OTHER;
}

/**
 * Splits a string give a delimter
 */
vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);

   while (getline(tokenStream, token, delimiter))
      tokens.push_back(token);

   return tokens;
}

/**
 * Checks if a file exists in the same directory
 */
bool fileExists(const string &filename)
{
    struct stat buffer;
    return stat(filename.c_str(), &buffer) == 0;
}

/**
 * Checks if the opened file is empty or not
 */
bool fileEmpty()
{
    struct stat buffer;
    int rc = fstat(fd, &buffer);
    return rc == 0 ? (buffer.st_size == 0) : false;
}

/**
 * Return the current hour and minute
 */
vector<int> getTime()
{
    vector<int> arr(2);
    const time_t lt = time(NULL);
    struct tm time = *localtime(&lt);
    arr[0] = time.tm_hour;
    arr[1] = time.tm_min;
    return arr;
}

// COMMAND FUNCTIONS //
void help()
{
    ifstream f("help.txt");
    if (f.is_open())
        cout << f.rdbuf();
    f.close();
}

void openfs(string filename)
{
    fd = open(filename.c_str(), 2);             // assign v6 filesystem a file descriptor
    lseek(fd, BLOCK_SIZE, SEEK_SET);            // set file pointer to beginning of superblock
    read(fd, &superBlock, sizeof(superBlock));  // read in superblock
    lseek(fd, BLOCK_SIZE, SEEK_CUR);            // set file pointer to beginning of root directory
	read(fd, &root_inode, sizeof(root_inode));  // read in root directory
    cout << "Successfully opened up V6 disk!";
}

void initfs(int total_blocks, int inode_blocks)
{
    // Fill in superBlock information
    superBlock.isize = inode_blocks;
    superBlock.fsize = total_blocks;

    int data_blocks = total_blocks - (inode_blocks + 3); // excluding root data block
    int free_blocks = data_blocks <= FREE_ARRAY_SIZE ? data_blocks : FREE_ARRAY_SIZE; // cap # of free data blocks at array size
    superBlock.nfree = free_blocks;
    int first_data_block = inode_blocks + 3;
    for(int i = 0; i < free_blocks; i++)
        superBlock.free[i] = first_data_block + i; // add free data blocks to free data block array
    
    int total_inodes = (BLOCK_SIZE / INODE_SIZE) * inode_blocks; // find total number of i nodes in the filesystem
    int free_inodes = total_inodes <= FREE_ARRAY_SIZE ? total_inodes - 1 : FREE_ARRAY_SIZE; // cap # of i-nodes
    superBlock.ninode = free_inodes;
    int first_free_inode = 2;
    for(int i = 0; i < free_inodes; i++)
        superBlock.inode[i] = first_free_inode + i; // add free i-nodes to free i-node array

    superBlock.flock = 0;
    superBlock.ilock = 0;
    superBlock.fmod = 0;
    superBlock.time = 0;

    // Write superBlock to disk
    lseek(fd, BLOCK_SIZE, SEEK_SET);
    write(fd, &superBlock, BLOCK_SIZE);

    // Initialize root directory (for i-node)
    root_inode.flags = 23039;
    root_inode.nlinks = 1;
    root_inode.uid = 0;
    root_inode.gid = 0;
    root_inode.size = 0;
    root_inode.addr[0] = inode_blocks + 2;
	root_inode.addr[1] = 0;

    vector<int> time = getTime();
	root_inode.actime[0] = time[0];
    root_inode.actime[1] = time[1];
	root_inode.modtime[0] = time[0];
	root_inode.modtime[1] = time[1];

    // Write root i-node to disk
    lseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
	write(fd, &root_inode, INODE_SIZE);

    // Initialize root directory (for data block)
    root_dir[0].inode = 1;
    strcpy((char*) root_dir[0].filename, ".");
    root_dir[1].inode = 1;
    strcpy((char*) root_dir[1].filename, "..");

    // Write root directory to disk
    lseek(fd, (first_data_block-1) * BLOCK_SIZE, SEEK_SET);
    write(fd, &root_dir[0], DIR_SIZE);
    lseek(fd, DIR_SIZE, SEEK_CUR);
    write(fd, &root_dir[1], DIR_SIZE);

    // Create empty blocks for all available data blocks
    char emptyBlock[BLOCK_SIZE] = {0};
    for(int i = 0; i < data_blocks; i++)
    {
        int blockNum = first_data_block + i;
        lseek(fd, blockNum * BLOCK_SIZE, SEEK_SET);
        write(fd, &emptyBlock, BLOCK_SIZE);
    }
    
    cout << "Successfully intiialized new V6 file system!";
}

void countFree()
{
    cout << "Number of Free Datablocks: " << superBlock.nfree << endl;
    cout << "Number of Free i-nodes: " << superBlock.ninode << endl;
}

// DRIVER CODE //
int main()
{
    // Display Help Screen For User
    string c = "";
    ifstream f("help.txt");
    if (f.is_open())
        cout << f.rdbuf();
    f.close();

    // Accept commands from user until they quit
    while (1)
    {
        cout << "\n" << endl;
        cout << "> ";
        getline(cin, c);

        vector<string> input = split(c, ' ');
        string command = input[0];
        transform(command.begin(), command.end(), command.begin(), ::tolower); // lowercase the command

        switch(hashC(command))
        {
            case QUIT:
                close(fd); exit(0);
            case HELP:
                help(); break;
            case OPENFS:
                if (input.size() != 2)
                {
                    cout << "Invalid Number of Arguments!";
                    break;
                }
                else if (!fileExists(input[1]))
                {
                    cout << "Cannot find filesystem!";
                    break;
                }
                openfs(input[1]); break;
            case INITFS:
                if (input.size() != 3)
                {
                    cout << "Invalid Number of Arguments!";
                    break;
                }
                else if (!fileEmpty())
                {
                    cout << "Filesystem is non-empty or you haven't opened it yet with the 'openfs' command!";
                    break;
                }
                initfs(stoi(input[1]), stoi(input[2])); break;
            case COUNTFREE:
                if (input.size() != 1)
                {
                    cout << "Invalid Number of Arguments!";
                    break;
                }
                else if (fileEmpty())
                {
                    cout << "Filesystem is empty or you haven't opened it yet with the 'openfs' command!";
                    break;
                }
                countFree(); break;
            case OTHER:
                cout << "Invalid Command!";
        }
    }
}