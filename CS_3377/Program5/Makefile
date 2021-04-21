#!/usr/bin/make -f
#
# Filename:		Makefile
# Date:			04/21/2020
# Author:		Sanjeev Penupala
# Email:		sxp170022@utdallas.edu
# Version:		1.0
# Copyright:	2020, All Rights Reserved
#
# Description:
#
#	A moderately complex makefile that takes care of 
#	creating dependencies
#

# Flags for the C++ implicit rules
CXX = g++
CPPFLAGS = -I include/
CXXFLAGS = -Wall
LDFLAGS = -L/home/013/s/sx/sxp170022/2.Sophomore/2nd_Semester/CS_3377/Program5/lib
LDLIBS = -lrudeconfig

# PROJECTNAME is a descriptive name used for the backup target
# This should not contain spaces or special characters
PROJECTNAME = program5

# C++ Source Files
SRCS = program5.cc parseCommandLine.cc parseConfigFile.cc processControl.cc signalHandling.cc inotifyFunctions.cc

# Name of Executable
EXEC = program5

#
# You normally don't need to change anything below here.
# ======================================================
#
OBJS = $(SRCS:cc=o)

# Declare Phony Targets
.PHONY: all clean

# Execute First Makefile Rule
all: $(EXEC)

# Clean Folder's Files
clean:
	rm -f $(OBJS) *.d *~ \#* *.pid *.log sxp170022_p5.tar.gz # $(EXEC)

# Rule To Link Files
$(EXEC): $(OBJS)
	$(CXX) -static -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

# Rule To Tar Program
tarball: clean
	tar cfvz sxp170022_p5.tar.gz ../Program5 -X exclude.txt

# Backup Target
111backup:	clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename "`pwd`"))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME) 2> /dev/null
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!

# Include the dependency files created by the PreProcessor.  The
# dash in front of the command keeps the system from complaining
# if the files do not exist.
# -include $(SRCS:.cc=.d)
