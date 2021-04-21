/*
 * Filename: 	program5.h
 * Date: 	    4/21/20
 * Author: 	  Sanjeev Penupala
 * Email: 	  sxp170022@utdallas.edu
 * Course: 	  CS 3377.501 Spring 2020
 * Version: 	1.0
 * Copyright 	2020, All Rights Reserved
 * 
 * Description:
 * 
 *    Header File With All Includes And Prototypes
 *
 */

#ifndef PROGRAM5_H
#define PROGRAM5_H

/* INCLUDES */
#include <iostream> 		// std::cout, std::cerr
#include <map> 			// std::map
#include <string> 		// std::string, substr();
#include <fstream> 		// std::ifstream, std::ofstream
#include <fcntl.h>      	// O_WRONLY, O_APPEND, O_CREAT
#include <unistd.h>		// fork(), exec(), setsid(), STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
#include <sys/stat.h>		// umask(), stat()
#include <sys/inotify.h>	// struct inotify_event
#include <stdio.h>		// popen(), remove(), sprintf()
#include <stdlib.h>		// ?
#include <cstdlib> 		// exit()
#include <signal.h>		// SIG_IGN
#include <linux/limits.h>	// PATH_MAX

#include "tclap/CmdLine.h"	// TCLAP
#include <rude/config.h>	// RUDE

/* NAMESPACES */
using namespace std;
using namespace TCLAP;
using namespace rude;

/* VARIABLES */
enum options {CONFIG_FILENAME, DAEMON, VERBOSE, LOGFILE, PASSWORD, NUMVERSIONS, WATCHDIR, RAW_NAME};	// ENUMS for Command Line Args

/* FUNCTIONS */

// parseCommandLine.cc
void parseCmdLine(int, char **, map<int, string> *);

// parseConfigFile.cc
void parseConfigFile(map<int, string> *);

// processControl.cc
void processControl(map<int, string> *);
void processControlD(map<int, string> *);
bool pidFileExists();
string executeCommand(string);
void reset();

// loggingFunctions.cc
void writeToFile(string, string);

// inotifyFunctions.cc
#define EVENT_SIZE              (sizeof (struct inotify_event))
#define BUF_LEN                 (1024 * (EVENT_SIZE + 16))

class InotifyFunctions
{
 private:
  int length, i;
  int fd;
  int wd;
  char buffer[BUF_LEN];

 public:
  InotifyFunctions(map<int, string> *); // Constructor
  void start(map<int, string> *);	// Start Reading In Changes
  void stop();				// Close Streams
};

// signalHandling.cc
void setVars(map<int, string> *, InotifyFunctions *);
void closeShop(int);
void reCONFigure(int);

#endif /* PROGRAM5_H */
