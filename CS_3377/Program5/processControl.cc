/*
 * Filename: 	processControl.cc
 * Date: 	    04/21/20
 * Author: 	  Sanjeev Penupala
 * Email: 	  sxp170022@utdallas.edu
 * Course: 	  CS 3377.501 Spring 2020
 * Version: 	1.0
 * Copyright 	2020, All Rights Reserved
 *
 * Description:
 * 
 *    Deals With Processes Created Using Fork()
 *
 */

#include "program5.h"

void processControl(map<int, string> *m)
{
  string print = "";
  bool verbose = (*m)[VERBOSE] == "true" ? true : false;
  pid_t forkvalue;
  
  if((*m)[DAEMON] == "true")			// If Daemon Mode Is True...
  {
    forkvalue = fork();				// Fork Off Parent Process (Create Child Process Or "Daemon")

    if(forkvalue < 0)				// ERROR: No Child Created!
    {
      print = verbose ? "> Error: No Child Was Created From The Fork" : "> Error (Fork)";
      cerr << print << endl;
      exit(1);
    }
    if(forkvalue > 0)				// SUCCESS: Parent Process Will Be Killed Soon!
    {
      print = verbose ? "> Parent Process Gracefully Killed!\n" : "";
      cout << print;
      fflush(stdout);
      sleep(1);
      exit(0);
    }
    
    print = verbose ? "> Created Daemon!\n" : "";
    cout << print;
    fflush(stdout);
  }

  // Create PID File
  ofstream pidFile;
  string pidFileName = (*m)[RAW_NAME] + ".pid";
  pidFile.open(pidFileName.c_str());
  if(pidFile.is_open())
  {
    pidFile << getpid() << endl;
    print = verbose ? "> Created PID File!\n" : "";
      cout << print;
    fflush(stdout);
  }
  else
  {
    print = verbose ? "> Error: Failed At Creating PID File" : "> Error (PID)";
    cerr << print << endl;
    exit(1);
  }
  pidFile.close();

  umask(0);					// Change File Mode Mask, So Daemon Can Write To Log File
  
  // Create File Descriptor For Log File And Make Sure It Is Opened
  int log;
  if((log = open((*m)[LOGFILE].c_str(), O_WRONLY|O_APPEND|O_CREAT, 0644)) < 0)
  {
    print = verbose ? "> Error: Cannot Create Log File" : "> Error (Log)";
    cerr << print << endl;
    exit(1);
  }
  print = verbose ? "> Log File Created!\n" : "";
  cout << print;
  fflush(stdout);
  
  // Create ".versions" folder
  if(executeCommand("find . -name '.versions' | wc -l") == "0\n")
  {
    string createDirCommand = "mkdir " + (*m)[WATCHDIR] + "/.versions";
    executeCommand(createDirCommand);
  }
  print = verbose ? "> '.versions' subfolder Initialized!\n" : "";
  cout << print;
  fflush(stdout);

  // Initialize Signals And Send Map To signalHandling.cc
  signal(SIGINT, closeShop);
  signal(SIGTERM, closeShop);
  signal(SIGHUP, reCONFigure);
  print = verbose ? "> Changed Behavior Of Kill Signals!\n" : "";
  cout << print;
  fflush(stdout);

  // Initialize inotifyFunctions
  InotifyFunctions inote(m);
  setVars(m, &inote);			  	// Send inotify obj to signalHandling.cc 
  print = verbose ? "> 'inotify' Initialized!\n" : "";
  cout << print;
  cout << "> Started Watching..." << endl;
  fflush(stdout);

  // Redirect STD_STREAMS
  close(STDIN_FILENO); 		 		// Close STDIN_FILENO descriptor
  dup2(log, STDOUT_FILENO);		        // Redirect STDOUT to Log File
  dup2(log, STDERR_FILENO);			// Redirect STDERR to Log File
  close(log);
  print = verbose ? "> Successfully Closed STDIN!\n" : "";
  print += verbose ? "> Successfully Redirected STDOUT & STDERR To Log File!\n" : "";
  cout << print;
  fflush(stdout);

  cout << "> Ready!" << endl;
  cout << "> " << endl;
  cout << "> ____________________(WATCHDIR LOGS)___________________" << endl;

  string date = executeCommand("date");		// Get Current Date and Time & 
  date = date.substr(0, date.length() - 1);	// Get Rid Of End Escape Character
  cout << "> Session Start Time: " << date << endl;
  cout << "> " << endl;
  fflush(stdout);

  // "The Big Loop"
  while(1)
  {
    // Do Tasks Here...
    inote.start(m);				// Watch Configured Directory
    sleep(1);
  }
}

string executeCommand(string command)
{
  FILE *output_from_command;
  char tmpbuffer[1024];
  string output = "";

  output_from_command = popen(command.c_str(), "r");

  if(!output_from_command)
    return "";

  while(fgets(tmpbuffer, 1024, output_from_command) != NULL)
  {
    output += tmpbuffer;
  }

  pclose(output_from_command);

  return output;
}

bool pidFileExists()
{
  string result = executeCommand("find . -name '*.pid' | wc -l");
  return result != "0\n";
}

void reset()
{
  // Resets State Of Project To Default Before Starting Another Daemon

  // If Cannot Find Default Config File Name...
  if(executeCommand("ls | grep cs3377dirmond.conf").length() == 0)	
  {
    string mvCommand = "mv *.conf cs3377dirmond.conf";
    executeCommand(mvCommand);
  }

  // executeCommand("rm -rf *.log");		// Remove Log File
}
