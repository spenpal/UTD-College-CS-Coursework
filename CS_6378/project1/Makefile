# Compiler and Compiler Flag Variables
JFLAGS = -g
JC = javac

# Java Source Files
SRCS = $(wildcard *.java)

# Java Class Files
CLS = $(SRCS:java=class)

# Declare Phony Targets
.PHONY: all program clean

# Execute First Makefile Rule
all: program

# Rule To Compile Files
program: $(CLS)

# Clean Folder's Files
clean:
	$(RM) *.class *~ sxp170022_p1.tar.gz

# Rule To Tar Program
tarball: clean
	tar -X exclude.txt -cvzf sxp170022_p1.tar.gz ../project1 

# How To Compile Files
%.class: %.java
	$(JC) $(JFLAGS) $<