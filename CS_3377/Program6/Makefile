#!/usr/bin/make -f
#
# Filename:		Makefile
# Date:			04/36/2020
# Author:		Sanjeev Penupala
# Email:		sxp170022@utdallas.edu
# Version:		1.0
# Copyright:		2020, All Rights Reserved
#
# Description:
#
#	A moderately complex makefile that takes care of 
#	creating dependencies
#

# Flags for the C++ implicit rules
CXX = g++
CXXFLAGS =
CPPFLAGS = -Wall -Wno-write-strings -g -I/scratch/perkins/include
LDFLAGS = -L/scratch/perkins/lib
LDLIBS = -lcdk -lcurses

# PROJECTNAME is a descriptive name used for the backup target
# This should not contain spaces or special characters
PROJECTNAME = CS_3377_Program6

# C++ Source Files
SRCS := $(wildcard *.cc)

# Name of Executable
EXEC = program6

#
# You normally don't need to change anything below here.
# ======================================================
#
OBJS = $(SRCS:.cc=.o)

# Declare Phony Targets
.PHONY: all clean

# Execute First Makefile Rule
all: $(EXEC)

# Clean Folder's Files
clean:
	rm -f $(OBJS) *.d *~ \#* $(EXEC)

# Rule To Tar Program
tarball: clean
	tar cfvz sxp170022_p6.tar.gz ../sxp170022_p6

# Rule To Link Files
$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

# Backup Target
backup:	clean
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
