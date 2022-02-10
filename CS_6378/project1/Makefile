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
	$(RM) *.class

# How To Compile Files
%.class: %.java
	$(JC) $(JFLAGS) $<