/*
 * Filename: convert.cc
 * Date: 2/13/20
 * Author: Sanjeev Penupala
 * Email: sxp170022@utdallas.edu
 * Course: CS 3377.501 Spring 2020
 * Version: 1.0
 * Copyright 2020, All Rights Reserved
 *
 * Description:
 * 
 *    Implementation Of "convert.h" header file
 *    Converts The Input File To Uppercase Or Lowercase, Based On What Switch Arg Was Passed
 *
 */

#include "convert.h"

void convertCase(string inFile, string outFile, bool upper, bool lower)
{
  // Intialize A Map To Store The Arg Values
  map<int, string> optionMap;
  enum files {INFILE, OUTFILE};
  optionMap[INFILE] = inFile;
  optionMap[OUTFILE] = outFile;

  // Open Input/Output Files
  ifstream myInputFile;
  ofstream myOutputFile;
  myInputFile.open(optionMap[INFILE].c_str(), ios::in);
  myOutputFile.open(optionMap[OUTFILE].c_str(), ios::out);

  string line; // Store Each Line Of Text

  if(myInputFile.is_open())
  {
    while(getline(myInputFile, line)) // While There Are Lines Left In The Input File
    {
      if(upper)
      {
	transform(line.begin(), line.end(), line.begin(), ::toupper); // Make Entire Line Into Uppercase
      }
      else if(lower)
      {
	transform(line.begin(), line.end(), line.begin(), ::tolower); // Make Entire Line Into Lowercase
      }
      
      myOutputFile << line << endl; // Add Line To The Output File
    }
  }

  myInputFile.close();
  myOutputFile.close();
}
