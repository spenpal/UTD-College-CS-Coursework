#!/usr/bin/make -f
#
# Filename:		Makefile
# Date:			03/31/2020
# Author:		Sanjeev Penupala
# Email:		sanjeev.penupala@utdallas.edu
# Version:		1.0
# Copyright:		2020, All Rights Reserved
#
# Description:
#
#	Builds a quick Lex Example Program.  Since Lex natively generates
#	C files, we will use the C compiler instead of the C++ compilar

#
# Flags for the C++ implicit rules
CC = gcc
CCFLAGS = -Wall
CPPFLAGS = 

# Flags for the Lex implicit rules
#
# Implicit Makefile rule to use Lex for C programs
#
#   n.c is made automatically from n.l by running Lex. The actual recipe is
#	$(LEX) $(LFLAGS)
LEX = /bin/flex
LFLAGS = 

# Implicit Makefile rule to use YACC for C programs
#
#   n.c is made automatically from n.y by running Yacc with the recipe
#       $(YACC) $(YFLAGS)
YACC = /bin/bison
YFLAGS = -dy

# PROJECTNAME is a descriptive name used for the backup target
# This should not contain spaces or special characters
PROJECTNAME = CS3377_Program4

EXECFILE = program4

OBJS = parse.o scan.o main.o

# Because we are insane, keep the intermediate files so we can look at
# them If we comment this out (or remove it), the intermediate scan.c
# file is automatically deleted.
# .PRECIOUS: scan.c

all: $(EXECFILE)

clean:
	rm -f $(OBJS) $(EXECFILE) *.P *~ \#* scan.c scanner parse.c y.tab.h parse.tab.c parser sxp170022_p4.tar.gz

$(EXECFILE):	$(OBJS)
	$(CC) -o $@ $(OBJS)
	ln -fs ./$@ scanner
	ln -fs ./$@ parser

tarball: clean
	tar cfvz sxp170022_p4.tar.gz ../Program4 -X exclude.txt

backup:
	@make clean
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

%o:%c
	$(CC) -M -MF $*P $<
	$(CC) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

# Includes any depedency files that were created by the preprocessor
-include $(OBJS:%.o=%.P)