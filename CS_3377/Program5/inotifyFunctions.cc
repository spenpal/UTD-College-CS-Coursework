/*
 * Filename: 	inotifyFunctions.cc
 * Date: 	    04/21/20
 * Author: 	  Sanjeev Penupala
 * Email: 	  sxp170022@utdallas.edu
 * Course: 	  CS 3377.501 Spring 2020
 * Version: 	1.0
 * Copyright 	2020, All Rights Reserved
 *
 * Description:
 * 
 *    File To Log Directory Changes To The WatchDir
 *
 */

/* INOTIFY_EVENT STRUCT
struct inotify_event 
{
  int wd;         	// The watch descriptor
  uint32_t mask;     	// Watch mask
  uint32_t cookie;    	// A cookie to tie two events together
  uint32_t len; 	// The length of the filename found in the name field
  char name __flexarr;	// The name of the file, padding to the end with NULs
}
*/

#include "program5.h"

InotifyFunctions::InotifyFunctions(map<int, string> *m)
{
  bool verbose = (*m)[VERBOSE] == "true" ? true : false;
  string print = "";

  // Initialize File Descriptor For "inotify"
  fd = inotify_init();
  if(fd < 0)
  {
    print = verbose ? "> Error: inotify_init() Did Not Initialize Properly" : "> Error (inotify_init)";
    cerr << print << endl;
    exit(1);
  }

  // Add Watches To inotify object
  wd = inotify_add_watch(fd, (*m)[WATCHDIR].c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
  if(wd == -1)
  {
    print = verbose ? "> Error: Could Not Add Watch(es) To inotify" : "> Error (inotify_watch)";
    cerr << print << endl;
    exit(1);
  }
} 

void InotifyFunctions::start(map<int, string> *m)
{
  bool verbose = (*m)[VERBOSE] == "true" ? true : false;
  string print = "";

  i = 0;
  length = read(fd, buffer, BUF_LEN);				// Read In Change(s)
  if(length < 0)
  {
    print = verbose ? "> Error: Did Not Read Changes Properly" : "> Error (inotify_read)";
    cerr << print << endl;
  }

  while(i < length)
  {
    struct inotify_event *event = (struct inotify_event *) &buffer[i];

    // Check If There Is At Least One Change Event
    if(event->len)
    {
      if(event->mask & IN_CREATE)				// If Directory Or File Is Created
      {
        if(event->mask & IN_ISDIR) 
        {
	  if(verbose)
	    printf("> The Directory '%s' Was Created!\n", event->name);
	  else
	    cout << "> Directory Created!" << endl;
	  fflush(stdout);
        }
        else 
        {
	  if(verbose)
	    printf("> The File '%s' Was Created!\n", event->name);
	  else
	    cout << "> File Created!" << endl;
	  fflush(stdout);
        }
      }
      else if(event->mask & IN_DELETE)				// If Directory Or File Is Deleted
      {
        if(event->mask & IN_ISDIR) 
        {
	  if(verbose)
	    printf("> The Directory '%s' Was Deleted!\n", event->name);
	  else
	    cout << "> Directory Deleted!" << endl;
	  fflush(stdout);
        }
        else 
        {
	  if(verbose)
	    printf("> The File '%s' Was Deleted!\n", event->name);
	  else
	    cout << "> File Deleted!" << endl;
	  fflush(stdout);
        }
      }
      else if(event->mask & IN_MODIFY)				// If Directory Or File Is Modified 
      {
	char Name[50];
	string name = "";	       				// Name Of Modified File/Directory
	string copyCommand = "";

        if(event->mask & IN_ISDIR) 				/* NOTE: THIS STATEMENT DOES NOT WORK */
        {
	  sprintf(Name, "%s", event->name);
	  if(verbose)
	    printf("> The Directory '%s' Was Modified!\n", Name);
	  else
	    cout << "> Directory Modified!" << endl;
	  fflush(stdout);
	  name = Name;
	  copyCommand = "cp -R " + (*m)[WATCHDIR] + "/" + name + "/ " + (*m)[WATCHDIR] + "/.versions/";	// "cp" command for directories
        }
        else 
        {
	  sprintf(Name, "%s", event->name);
	  if(verbose)
	    printf("> The File '%s' Was Modified!\n", Name);
	  else
	    cout << "> File Modified!" << endl;
	  fflush(stdout);
	  name = Name;
	  copyCommand = "cp " + (*m)[WATCHDIR] + "/" + name + " " + (*m)[WATCHDIR] + "/.versions/";	// "cp" command for files
        }

	string date = executeCommand("date +%Y.%m.%d-%T");	// Get Current Date and Time
	date = date.substr(0, date.length() - 1);		// Get Rid Of Ending Escape Character
	string newName = name + "." + date;			// Append Current Date And Time To Modified File/Dir Name
	copyCommand += event->mask & IN_ISDIR ? newName + "/" : newName;
	executeCommand(copyCommand);				// Copy Modified File To ".versions" folder
      }
    }

    i += EVENT_SIZE + event->len;
  }
}

void InotifyFunctions::stop()
{
  inotify_rm_watch(fd, wd);
  close(fd);
}
