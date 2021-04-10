/*
 * Filename: main.c
 * Date: 4/3/20
 * Author: Sanjeev Penupala
 * Email: sxp170022@utdallas.edu
 * Course: CS 3377.501 Spring 2020
 * Version: 1.0
 * Copyright 2020, All Rights Reserved
 *
 * Description:
 * 
 *    The goal of the program is to scan and parse an input file that contains postal addresses and then to output a
 *    representation of those addresses in XM format
 *
 */

#include <stdio.h>    //  printf()
#include <string.h>   //  strcmp()
#include "y.tab.h"    //  TOKENS

/* 
 * Manually insert prototype and extern declarations for the
 * stuff in the flex output file.
 */
int yylex(void);
extern char *yytext;

/* Just call the lexical scanner until we reach EOF */
int main(int argc, char *argv[])
{
  int token;
  char scan[] = "./scanner";
  char parse[] = "./parser";
  
  if(strcmp(scan, argv[0]) == 0)
  {
    printf("\nOperating in scan mode...\n");
    
    token = yylex();

    while(token != 0)
    {
      printf("\nyylex returned ");

      switch(token)
      {       
        case COMMATOKEN:
	  printf("COMMATOKEN token (%d)", token);
	  break;
	case DASHTOKEN:
	  printf("DASHTOKEN token (%d)", token);
	  break;
        case EOLTOKEN:
	  printf("EOLTOKEN token (%d)", token);
	  break;
	case HASHTOKEN:
  	  printf("HASHTOKEN token (%d)", token);
          break;
	case JRTOKEN:
	  printf("JRTOKEN token (%s)", yytext);
	  break;
	case SRTOKEN:
	  printf("SRTOKEN token (%s)", yytext);
	  break;
	case IDENTIFIERTOKEN:
	  printf("IDENTIFIERTOKEN token (%s)", yytext);
	  break;
	case INTTOKEN:
	  printf("INTTOKEN token (%s)", yytext);
	  break;
	case ROMANTOKEN:
	  printf("ROMANTOKEN token (%d)", token);
	  break;
        case NAMETOKEN:
	  printf("NAMETOKEN token (%s)", yytext);
	  break;
        case NAME_INITIAL_TOKEN:
	  printf("NAME_INITIAL_TOKEN token (%s)", yytext);
	  break;
	default:
	  printf("UNKNOWN");
      }
    
      token = yylex();
    }

    printf("\n\nScan Successful!\n\n");
  }
  else if(strcmp(parse, argv[0]) == 0)
  {
    printf("Operating in parse mode...\n\n");
    yyparse();
    printf("\nParse Successful!\n");
  }
  else
  {
    printf("Invalid Soft Link!\n\n");
  }

  return 0;
}