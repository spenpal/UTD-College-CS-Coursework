/*
 * Filename: Sanjeev Penupala
 * Date: 2/26/20
 * Author: Sanjeev Penupala
 * Email: sxp170022@utdallas.edu
 * Course: CS 3377.501 Spring 2020
 * Version: 1.0
 * Copyright 2020, All Rights Reserved
 *
 * Description:
 * 
 *    Program 3 takes a simple gawk program that calculates and outputs the sum of specific columns and displays it
 *    back to the user. Afterwards, it will take those 2 columns sums and output an overall sum
 *
 */

#include "gawk.h"

int main(int argc, char *argv[])
{
  // Print Command Locations
  string workDir = printComOut("pwd");
  workDir = workDir.substr(0, workDir.length() - 2);
  
  cout << "gawk at: "  << workDir << "/bin/gawk" << endl;
  cout << "Shellcmd1: " << workDir << "/bin/gawk --version" << endl;
  cout << "Shellcmd2: " << workDir << "/bin/gawk -f gawk.code numbers.txt" << endl;
  cout << endl;

  // Print Out Version Of Gawk
  cout << "The first call to gawk returned:" << endl;
  cout << endl;
  cout << printComOut("gawk --version");
  cout << endl;

  // Print Out Summation Of Columns 1 And 4
  cout << "The second call to gawk returned: ";
  string sums = printComOut("gawk -f gawk.code numbers.txt");
  cout << sums << endl;

  size_t space = sums.find(" ");
  string colOneSum = "", colFourSum = "";
  int sumOne, sumFour;

  if(space != string::npos)
  {
    colOneSum = sums.substr(0, space);
    colFourSum = sums.substr(space);
  }

  stringstream s1(colOneSum);
  stringstream s4(colFourSum);
  s1 >> sumOne;
  s4 >> sumFour;

  cout << "The sum of Column 1 is: " << sumOne << endl;
  cout << "The sum of Column 4 is: " << sumFour << endl;
  cout << "The sum of the two numbers is: " << (sumOne + sumFour) << endl;

  return 0;
}
