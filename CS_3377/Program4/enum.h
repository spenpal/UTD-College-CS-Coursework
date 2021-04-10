/*
 * Filename: enum.h
 * Date: 3/29/20
 * Author: Sanjeev Penupala
 * Email: sxp170022@utdallas.edu
 * Course: CS 3377.501 Spring 2020
 * Version: 1.0
 * Copyright 2020, All Rights Reserved
 *
 * Description:
 *
 *     An enum that contains all classifers for encountered strings
 *
 */

#ifndef ENUM_H
#define ENUM_H

/* Enumerate the tokens we will try to match.  Make sure to NOT have a token with value 0 */
enum {COMMATOKEN=1, DASHTOKEN, EOLTOKEN, HASHTOKEN, JRTOKEN, SRTOKEN, INTTOKEN, ROMANTOKEN, NAMETOKEN, NAME_INITIAL_TOKEN, IDENTIFIERTOKEN};

#endif /* ENUM_H */
