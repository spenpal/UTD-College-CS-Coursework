/*
 * Filename: gawk.h
 * Date: 3/1/20
 * Author: Sanjeev Penupala
 * Email: sxp170022@utdallas.edu
 * Course: CS 3377.501 Spring 2020
 * Version: 1.0
 * Copyright 2020, All Rights Reserved
 *
 * Description:
 *
 *    Define functions used in gawk.cc
 *
 */

#ifndef GAWK_H
#define GAWK_H
#define BUFFER_SIZE 1024 // Max Size Of A Line To Read From The Pipe

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

string printComOut(string);

#endif /* GAWK_H_ */
