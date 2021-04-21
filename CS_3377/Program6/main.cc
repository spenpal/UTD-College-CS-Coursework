/*
 * Filename:	main.cc
 * Date: 	04/26/2020
 * Author: 	Sanjeev Penupala
 * Email: 	sxp170022@utdallas.edu
 * Course: 	CS 3377.501 Spring 2020
 * Version: 	1.0
 * Copyright 	2020, All Rights Reserved
 *
 * Description:
 *
 *	You will be using binary file I/O techniques to open a supplied binary file.
 *	The file consists of a header record that is immediately followed by a set of data records.
 *	You are to read and display the header record.
 *	Using information in the header record, you will read the data records from the file.
 *	For each data record, you are to display the results in a CDK window.
 *	After display, you are to wait until the user types a character on the keyboard. When they do, you will exit the program.
 *     
 */

#include "program6.h"

int main()
{
  string matrix[MATRIX_HEIGHT + 1][MATRIX_WIDTH + 1];				// Stores All Information From Binary File
  ifstream file("/scratch/perkins/cs3377.bin", ios::in | ios::binary);
  
  if(!file.is_open())
  {
    cerr << "Error: File Did Not Open" << endl;
    exit(EXIT_FAILURE);
  }
  
  // HEADER RECORDS //
  BinaryFileHeader *header = new BinaryFileHeader();
  
  if(!file.read((char *) header, sizeof(BinaryFileHeader)))
  {
    cerr << "Error: Reading Corrupted" << endl;
    cerr << "Debugging..." << endl;
    cerr << "# of Bytes Read (Before Error): " << file.gcount() << endl;
    exit(EXIT_FAILURE);
  }

  stringstream ssh;								// "stringstream" Object For Header Record

  // Place "Number of Records" Into Matrix
  ssh << "NumRecords: " << header->numRecords;
  matrix[1][3] = ssh.str();
  ssh.str("");									// Clears "stringstream"
  ssh.clear();
  
  // Place "Version Number" Into Matrix
  ssh << "Version: " << header->versionNumber;
  matrix[1][2] = ssh.str();
  ssh.str("");
  ssh.clear();
  
  // Place "Magic Number" Into Matrix
  ssh << "Magic: 0x" << hex << uppercase <<  header->magicNumber;		// Convert Decimal Number To Hex
  matrix[1][1] = ssh.str();
  ssh.str("");
  ssh.clear();

  // DATA RECORDS // 
  stringstream ssd;								                                        // "stringstream" Object For Data Record
  BinaryFileRecord *record = new BinaryFileRecord();
  uint64_t numRecords = header->numRecords < 4 ? header->numRecords : 4;	// Read Max Of 4 Records From Binary File

  for(uint64_t i = 2; i < numRecords + 2; i++)
  {
    if(!file.read((char *) record, sizeof(BinaryFileRecord)))
    {
      cerr << "Error: Reading Corrupted" << endl;
      cerr << "Debugging..." << endl;
      cerr << "# of Bytes Read (Before Error): " << file.gcount() << endl;
      exit(EXIT_FAILURE);
    }
    
    ssd << "strlen: " << (int) record->strLength;				// Convert "unsigned char" To "int"

    matrix[i][1] = ssd.str();							              // Length of Data
    matrix[i][2] = record->stringBuffer;					      // Data

    ssd.str(""); 							    	                    // Clear stringstream
    ssd.clear();
  }

  file.close();
  display(matrix);	// Sends Data To CDK Matrix, To Be Displayed

  return 0;
}
