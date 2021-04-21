/*
 * Filename: 	program5.cc
 * Date: 	    04/21/2020
 * Author: 	  Sanjeev Penupala
 * Email: 	  sxp170022@utdallas.edu
 * Course: 	  CS 3377.501 Spring 2020
 * Version: 	1.0
 * Copyright 	2020, All Rights Reserved
 *
 * Description:
 * 
 *   	The goal of this program is to create a UNIX style daemon process that will watch a folder and
 *	respond to specific file operations that occur in that folder. The daemon process will read its
 * 	configuration parameters from a CONFIG file and then operate according to that configuration. The
 * 	daemon process will also respond to several UNIX signals that will help control its operation.
 *
 */

#include "program5.h"

int main(int argc, char **argv)
{
  // Check To See If There Is An Existing Daemon
  if(pidFileExists())
  {
    cerr << "> Error: Existing Daemon Already Running! Cannot Create A New One!" << endl;
    exit(1);
  }
  else
  {
    reset();					// Always Reset To The Default State Of The Program
    cout << "> Resetting Program To Default State..." << endl;
    fflush(stdout);
  }

  map<int, string> optionMap;			// Intialize A Map To Store The Values
  parseCmdLine(argc, argv, &optionMap);		// Parse Command Line For Flag Arg
  cout << "> Parsed Command Line For Arguments!" << endl;
  fflush(stdout);
  
  // Rename Current Config File To New Config Filename
  if(optionMap[CONFIG_FILENAME] != "cs3377dirmond.conf")
  {
    string mvCommand = "mv *.conf " + optionMap[CONFIG_FILENAME];
    executeCommand(mvCommand);
  }
  
  parseConfigFile(&optionMap);			// Parse Config File For Parameters Of Daemon
  cout << "> Parsed Configuration File For Parameters!" << endl;
  fflush(stdout);
  
  /* Debugging
  cout << "Config Filename: "	<< 	optionMap[CONFIG_FILENAME]	<< endl;
  cout << "Daemon: " 		<< 	optionMap[DAEMON] 		<< endl;
  cout << 								   endl;
  cout << "Verbose: " 		<< 	optionMap[VERBOSE] 		<< endl;
  cout << "LogFile: " 		<< 	optionMap[LOGFILE] 		<< endl;
  cout << "Password: " 		<< 	optionMap[PASSWORD] 		<< endl;
  cout << "NumVersions: " 	<< 	optionMap[NUMVERSIONS] 		<< endl;
  cout << "Watchdir: " 		<< 	optionMap[WATCHDIR] 		<< endl;
  */

  processControl(&optionMap);			// Main Flow Of Program

  return 0;
}
