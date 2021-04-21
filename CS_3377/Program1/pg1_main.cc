/*
 * Filename: pg1_main.cc
 * Date: 1/30/20
 * Author: Sanjeev Penupala
 * Email: sxp170022@utdallas.edu
 * Course: CS 3377.501 Spring 2020
 * Version: 1.0
 * Copyright: 2020, All Rights Reserved
 *
 * Description
 * 
 *    Contains the main() method for this program
 *    Prints the number of arguments given
 *    Prints the value of the arguments
 */

#include "pg1_func.h"

int main(int argc, char *argv[])
{
  cout << "argc was: " << argc << endl;
  
  for(int i = 0; i < argc; i++)
  {
    cout << argv[i] << endl;
  }

  cout << "Done!" << endl;

  proc1();

  return 0;
}
