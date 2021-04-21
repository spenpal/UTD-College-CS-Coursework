/*
 * Filename: gawk.cc
 * Date: 3/1/20
 * Author: Sanjeev Penupala
 * Email: sxp170022@utdallas.edu
 * Course: CS 3377.501 Spring 2020
 * Version: 1.0
 * Copyright 2020, All Rights Reserved
 *
 * Description:
 * 
 *    Run any command and brings the output to the C++ program
 *
 */

#include "gawk.h"

string printComOut(string command)
{
  FILE *output_from_command;
  char tmpbuffer[BUFFER_SIZE];
  string output = "";

  output_from_command = popen(command.c_str(), "r");

  if(!output_from_command)
    return "";

  while(fgets(tmpbuffer, BUFFER_SIZE, output_from_command) != NULL)
  {
    output += tmpbuffer;
  }

  pclose(output_from_command);
  
  return output;
}
