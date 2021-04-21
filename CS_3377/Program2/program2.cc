/*
 * Filename: program2.cc
 * Date: 2/13/20
 * Author: Sanjeev Penupala
 * Email: sxp170022@utdallas.edu
 * Course: CS 3377.501 Spring 2020
 * Version: 1.0
 * Copyright 2020, All Rights Reserved
 *
 * Description:
 * 
 *    Program takes in an input file with words and either turns everything into uppercase or lowercase,
 *    and writes the output to a new file
 *
 *    This program uses TCLAP, a customized command line argument parser, to implement flags that will allow the user to
 *    select whether or not they want the input file to be all uppercase or all lowercase
 *
 */

#include "tclap/CmdLine.h"
#include "convert.h"

using namespace TCLAP;

int main(int argc, char *argv[])
{
  try
  {
    // Intializing All CMD Arg Objects
    CmdLine cmd("CS 3376.501 Program 2", ' ',  "4.8.5");
    UnlabeledValueArg<string> infile("infile", "Input File Name", true, "input.txt", "Input Filename", false);
    SwitchArg upperCase("u", "upper", "Convert All Text To Uppercase", cmd, false);
    SwitchArg lowerCase("l", "lower", "Convert All Text To Lowercase", cmd, false);
    ValueArg<string> outfile("o", "outfile", "Name Of The Output File", false, "output.txt", "output filename");
    
    // Add All Args, Except SwitchArgs To The cmd Object
    cmd.add(infile);
    cmd.add(outfile);

    cmd.parse(argc, argv); // Parse The Command Line And Get All Flags And Args
    
    bool upper = upperCase.getValue();
    bool lower = lowerCase.getValue();
    
    // Check If Both Uppercase and Lowercase Are True
    if(upper && lower)
    {
      cerr << "Error: Can't Make Text Both Uppercase and Lowercase! Try again! Here is the proper usage: " << endl;
      cmd.getOutput()->usage(cmd);
      exit(1);
    }

    // Converts Input File To Either Uppercase, Lowercase, Or Neither To An Output File
    convertCase(infile.getValue(), outfile.getValue(), upper, lower);
    return 0;
  }
  catch(ArgException &e)
  {
    cerr << "Error: " << e.error() << " for arg " << e.argId() << endl;
  }
}
