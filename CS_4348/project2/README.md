# Project 2

## Contributors:
- Sanjeev Penupala \<SXP170022> 
- Gautam Sapre \<GSS170001>

## Contributions
- **Sanjeev** implemented all the commands asked on the instructions.
- **Gautam** worked on the main method to ensure the user understands how to run commands at runtime of the program, in an intuitive UI. He also worked on refactoring code and ensuring there is consistency in naming conventions.

## How to Run Project
- First, create a empty disk file.
```
g++ -std=c++11 -o v6 mod-v6.cc
./v6
```

# Tarball Command
`tar cvzf sxp170022_gss170001_project.tar.gz mod-v6.cc help.txt README.md`

# Help
```
Usage:
  <command> [arguments]

Commands:
  openfs <filename>                     Opens up the file that represents the disk drive.
  initfs <n1> <n2>                      Set unallocated blocks to free (except i-node 1).
  count-free                            Returns the number of free data blocks and i-nodes available in the system.
  quit                                  Quits the running V6 program.
  help                                  Prints the help page.
    
Arguments:
  filename                              A local file where the program is being run that represents the disk drive.
  n1                                    The file system size in number of blocks.
  n2                                    The number of blocks devoted to the i-nodes.
```