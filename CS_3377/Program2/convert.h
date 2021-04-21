/*
 * Filename: convert.h
 * Date: 2/13/20
 * Author: Sanjeev Penupala
 * Email: sxp170022@utdallas.edu
 * Course: CS 3377.501 Spring 2020
 * Version: 1.0
 * Copyright 2020, All Rights Reserved
 * 
 * Description:
 * 
 *    Header File Defining The Function To Convert The Input File To Uppercase Or Lowercase Or Neither
 *
 */

#ifndef CONVERT_H
#define CONVERT_H

#include <iostream> // std::cout, std::cerr
#include <map> // std::map
#include <string> // std::string
#include <fstream> // std::ifstream, std::ofstream
#include <algorithm> // std::transform
#include <cstdlib> // exit()

using namespace std;

void convertCase(string, string, bool, bool);

#endif /* CONVERT_H */
