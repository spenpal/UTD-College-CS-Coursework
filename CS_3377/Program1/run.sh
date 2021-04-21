#!/bin/bash

# Filename: run.sh
# Date: 1/30/2020
# Author: Sanjeev Penupala
# Email: sxp170022@utdallas.edu
# Course: Cs 3377.501 Spring 2020
# Version: 1.0
# Copyright: 2020, All Rights Reserved
#
# Description
#
#    run.sh will execute the program multiple times with different arguments each time
# 

echo "Running 'pg1' with 0 arguments:"
./pg1 > stdout.log 2> stderr.log
echo "    stdout appended to stdout.log"
echo "    stderr appended to stderr.log"

echo "Running 'pg1' with 1 argument:"
./pg1 abc >> stdout.log 2>> stderr.log
echo "    stdout appended to stdout.log"
echo "    stderr appended to stderr.log"

echo "Running 'pg1' with 5 arguments:"
./pg1 a b c d e >> stdout.log 2>> stderr.log
echo "    stdout appended to stdout.log"
echo "    stderr appended to stderr.log"
