# Assignment 6

## Purpose
> Demonstrate the ability to create a multi-file project that is hosted in a GIT repository. Demonstrate
the ability to perform Binary File I/O. Demonstrate the ability to visualize binary data using a CDK
matrix. 

## Overview
> You will be using binary file I/O techniques to open a supplied binary file. The file consists of a
header record that is immediately followed by a set of data records. You are to read and display the
header record. Using information in the header record, you will read the data records from the file.
For each data record, you are to display the results in a CDK window. After display, you are to wait
until the user types a character on the keyboard. When they do, you will exit the program.


## More Information
> example1.cc : This example shows how to create a small matrix using CDK.

It provides this info:
   1) CDK Setup
   2) Creating a CDK Matrix
   3) Placing txt info into a matrix box

Please note these items to students:
   1) -I and -L flags to compiler link against /scratch/perkins/include and /scratch/perkins/lib respectively
   2) Linking with the CDK and curses libraries via these commpiler flags: -lcdk -lcurses
   3) The arrays of characters need to be defined as (const char **) However, the newCDKMatrix wants (char **).  Note\
 manual cast.
   4) A matrix starts at location 1,1.  Arrays start at location 0.  Therefore arrays are one longer than they should\
 be and the [0] elements are not used.

