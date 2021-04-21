#!/bin/bash

# Filename: comrun.sh
# Date: 2/13/20
# Author: Sanjeev Penupala
# Email: sxp170022@utdallas.edu
# Course: CS 3377.501 Spring 2020
# Version: 1.0
# Copyright 2020, All Rights Reserved
# 
# Description:
# 
#    Compile Program 2 with included path to the CmdLine header file
#    Run Program 2, given args for the proper usage
#

echo "Compiling Program 2..."
g++ -c -I include/ program2.cc
g++ -c convert.cc

echo "Linking all .cc files together..."
g++ program2.o convert.o -o program2
echo "Successfully compiled Program 2!"

echo "Running Program 2 with -h flag..."
./program2 -h

echo "Running Program 2 with --version flag..."
./program2 --version

echo "Running Program 2 with input file only..."
./program2 input.txt
cat output.txt

echo "-------------------------------------------------------------------------------------------------------------"
echo "Running Program 2 with input and output file only..."
./program2 input.txt -o o1.txt
cat o1.txt

echo "-------------------------------------------------------------------------------------------------------------"
echo "Running Program 2 with input, output, and -u flag..."
./program2 input.txt -u -o o2.txt
cat o2.txt

echo "-------------------------------------------------------------------------------------------------------------"
echo "Running Program 2 with input, output, -l..."
./program2 input.txt -l -o o3.txt
cat o3.txt

echo "-------------------------------------------------------------------------------------------------------------"
echo "Running Program 2 with input, -u and -l..."
./program2 input.txt -l -u

echo "-------------------------------------------------------------------------------------------------------------"
