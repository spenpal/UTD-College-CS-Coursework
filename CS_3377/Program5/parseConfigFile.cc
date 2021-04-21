/*
 * Filename: 	parseConfigFile.cc
 * Date: 		04/21/20
 * Author: 		Sanjeev Penupala
 * Email: 		sxp170022@utdallas.edu
 * Course: 		CS 3377.501 Spring 2020
 * Version: 	1.0
 * Copyright 	2020, All Rights Reserved
 *
 * Description:
 * 
 *    Parses Given Config File For Its Parameters
 *
 */

#include "program5.h"

void parseConfigFile(map<int, string> *paramMap)
{
  Config config;				// Create Config Object
  bool verbose = true;
  string print = "";

  // Load Config File
  if(config.load((*paramMap)[CONFIG_FILENAME].c_str()))
  {
    if(config.setSection("Parameters", false))	// Check If "Parameters" Section Exists
    {
      if(config.getNumDataMembers() >= 5) 	// Check If All Parameters Exist
      {
	try
	{
	  // Update LogFile Name, If Config Filename Changed
	  string logfile = (*paramMap)[RAW_NAME] + ".log";
	  config.setStringValue("LogFile", logfile.c_str());
	  config.save();
	
	  // Place Config File Parameters Into Map
	  (*paramMap)[VERBOSE] = 	config.getStringValue("Verbose");
	  verbose = (*paramMap)[VERBOSE] == "true" ? true : false;
	  (*paramMap)[LOGFILE] = 	config.getStringValue("LogFile");
	  (*paramMap)[NUMVERSIONS] = 	config.getStringValue("NumVersions");

	  bool passwordAlt = (*paramMap)[PASSWORD].length() == 0 ? false : (*paramMap)[PASSWORD] != config.getStringValue("Password");
	  bool watchDirAlt = (*paramMap)[WATCHDIR].length() == 0 ? false : (*paramMap)[WATCHDIR] != config.getStringValue("WatchDir");

	  // Check If Password And/Or WatchDir Has Been Altered
	  if(passwordAlt || watchDirAlt)
	  {
	    if(passwordAlt)
	    {
	      print = verbose ? "> Error: Cannot Change \"Password\" In Config File Once Set" : "> Error (Password)";
	      cerr << print << endl;
	    }
	    if(watchDirAlt)
	    {
	      print = verbose ? "> Error: Cannot Change \"WatchDir\" In Config File Once Set" : "> Error (WatchDir)";
	      cerr << print << endl;
	    }
	    closeShop(1);
	  }
	  else				   	// Update Password And WatchDir For First Time Parse Of Config File
	  {
	    (*paramMap)[PASSWORD] = 	config.getStringValue("Password");
	    (*paramMap)[WATCHDIR] = 	config.getStringValue("WatchDir");
	  }
	}
	catch(...)
	{
	  // Error Parameter Names Have Been Modified
	  print = verbose ? "> Error: Unknown Parameter(s) In Config File" : "> Error (Parameter_Names)";
	  cerr << print << endl;
	  closeShop(1);
	}
      }
      else
      {
	// Error All Parameters Are Not In The Config File
	print = verbose ? "> Error: All Parameters Are Not Loaded" : "> Error (Parameters_Missing)";
	cerr << print << endl;
	closeShop(1);
      }
    }
    else
    {
      // Error Retrieving "Parameters" Section
      print = verbose ? "> Error: \"Parameters\" Section Does Not Exist" : "> Error (Parameter_Section)";
      cerr << print << endl;
      closeShop(1);
    }
  }
  else
  {
    // Error Loading The Config File Failed
    print = verbose ? "> Error: Config File Could Not Be Loaded" : "> Error (Config_File)";
    cerr << print << endl;
    closeShop(1);
  }
}
