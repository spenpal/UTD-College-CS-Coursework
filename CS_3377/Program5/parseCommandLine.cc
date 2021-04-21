/*
 * Filename: 	parseCommandLine.cc
 * Date: 	    04/21/20
 * Author: 	  Sanjeev Penupala
 * Email: 	  sxp170022@utdallas.edu
 * Course: 	  CS 3377.501 Spring 2020
 * Version: 	1.0
 * Copyright 	2020, All Rights Reserved
 *
 * Description:
 * 
 *    Parses The Command Line For Arguments
 *
 */

#include "program5.h"

void parseCmdLine(int argc, char **argv, map<int, string> *argsMap)
{
  try
  {
    // Intializing All CMD Arg Objects
    CmdLine cmd("CS 3377.501 Program 5", ' ',  "1.0");
    UnlabeledValueArg<string> configFile("config_filename", "The name of the configuration file. Defaults to cs3377dirmond.conf", false, "cs3377dirmond.conf", "Configuration File", true);
    SwitchArg dMode("d", "daemon", "Run in daemon mode (forks to run as a daemon).", cmd, false);

    cmd.add(configFile);        // Add Config Arg To "cmd" object
    cmd.parse(argc, argv);      // Parse The Command Line And Get Args
    
    // Put Args Into optionMap
    (*argsMap)[CONFIG_FILENAME] = configFile.getValue();
    int len = configFile.getValue().length();
    (*argsMap)[RAW_NAME] = configFile.getValue().substr(0, len - 5);
    (*argsMap)[DAEMON] = dMode.getValue() ? "true" : "false";
  }
  catch(ArgException &e)
  {
    // Error
    cerr << "Error: " << e.error() << " for arg " << e.argId() << endl;
  }
}
