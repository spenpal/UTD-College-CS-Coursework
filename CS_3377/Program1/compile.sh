#!/bin/bash

# Filename: compile.sh
# Date: 1/30/2020
# Author: Sanjeev Penupala
# Email: sxp170022@utdallas.edu
# Course: CS 3377.501 Spring 2020
# Version: 1.0
# Copyright: 2020, All Rights Reserved
#
# Description
#
#   Compile And Link All Files Of The Program
#

#echo "Setting TMPDIR environment variable to /scratch"
#TMPDIR=/scratch; export TMPDIR

echo "Compiling pg1_main.cc"
g++ -c pg1_main.cc
echo "Compiling pg1_func.cc"
g++ -c pg1_func.cc
echo "Linking files to create executable pg1"
g++ pg1_main.o pg1_func.o -o pg1
