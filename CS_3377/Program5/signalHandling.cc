/*
 * Filename: 	signalHandling.cc
 * Date: 	    04/21/20
 * Author: 	  Sanjeev Penupala
 * Email: 	  sxp170022@utdallas.edu
 * Course: 	  CS 3377.501 Spring 2020
 * Version: 	1.0
 * Copyright 	2020, All Rights Reserved
 *
 * Description:
 * 
 *    Sends Various Signals To The Daemon
 *
 */

#include "program5.h"

map<int, string> *thisMap;
InotifyFunctions *obj;

void setVars(map<int, string> *m, InotifyFunctions *o)
{
  thisMap = m;
  obj = o;
}

void closeShop(int signo)
{
  bool verbose = (*thisMap)[VERBOSE] == "true" ? true : false;
  string print = "";

  string pid = (*thisMap)[RAW_NAME] + ".pid";
  remove(pid.c_str());	// Remove PID File
  cout << "> " << endl;
  print = verbose ? "> Removed PID File!\n" : "";
  cout << print;
  fflush(stdout);

  obj->stop();
  print = verbose ? "> Removed Watches And Closed inotify!\n" : "";
  cout << print;
  fflush(stdout);
  
  cout << "> " << endl;
  cout << "> *Program End*" << endl;

  string date = executeCommand("date");         // Get Current Date and Time &
  date = date.substr(0, date.length() - 1);     // Get Rid Of End Escape Character
  cout << "> Session End Time: " << date << endl;
  cout << "> " << endl;
  cout << "> %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
  cout << "> " << endl;

  fflush(stdout);
  exit(1);
}

void reCONFigure(int signo)
{
  parseConfigFile(thisMap);   	// Re-Configure Daemon With Config File Again
  cout << "> " << endl;
  cout << "> Reconfigured Parameters!" << endl;
  cout << "> " << endl;
  fflush(stdout);
}
