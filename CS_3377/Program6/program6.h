/*
 * Filename:    program6.h
 * Date:        04/26/2020
 * Author:      Sanjeev Penupala
 * Email:       sxp170022@utdallas.edu
 * Course:      CS 3377.501 Spring 2020
 * Version:     1.0
 * Copyright    2020, All Rights Reserved
 *
 * Description:
 *
 *	Header File
 *
 */

#ifndef PROGRAM6_H
#define PROGRAM6_H

#include <iostream>     // std::cout, std::cerr
#include <fstream>      // std::ifstream
#include <string>       // std::string
#include <sstream>      // std::stringstream
#include <stdio.h>      // getchar()
#include <stdlib.h>     // exit(), EXIT_FAILURE
#include <stdint.h>	    // uint8_t, uint32_t, uint64_t
#include "cdk.h"

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

/* HEADER RECORD FROM BINARY FILE */
class BinaryFileHeader
{
  public:
    uint32_t magicNumber;               /* Should be 0xFEEDFACE */
    uint32_t versionNumber;
    uint64_t numRecords;
};

/* DATA RECORD FROM BINARY FILE */
const int maxRecordStringLength = 25;

class BinaryFileRecord
{
  public:
    uint8_t strLength;
    char stringBuffer[maxRecordStringLength];
};

/* FUNCTIONS */
void display(string [MATRIX_HEIGHT + 1][MATRIX_WIDTH + 1]);

#endif /* PROGRAM6_H */
