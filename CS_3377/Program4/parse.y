/*
 * Filename:		parse.y
 * Date:		04/03/2020
 * Author:	        Sanjeev Penupala
 * Email:		sxp170022@utdallas.edu
 * Version:		1.0
 * Copyright:		2020, All Rights Reserved
 *
 * Description:
 *
 *	Bison Program
 */

/* C++ Program In Parse File */
%{
  #include <stdio.h>

  int yylex(void);
  void yyerror(char *s);
%}

%union {
  	int num;
  	char *str;
}

%token		COMMATOKEN
%token		DASHTOKEN
%token		EOLTOKEN
%token		HASHTOKEN
%token		JRTOKEN
%token		SRTOKEN
%token	<str> 	IDENTIFIERTOKEN
%token 	<num> 	INTTOKEN
%token 	<str> 	ROMANTOKEN
%token 	<str> 	NAMETOKEN
%token	<str>	NAME_INITIAL_TOKEN

%start 	prog

%%

prog		:	postal_addresses
		;

postal_addresses:	address_block EOLTOKEN { fprintf(stderr, "\n"); } postal_addresses
	        |	address_block
	        ;

address_block	:	name_part street_address location_part
	     	;

name_part	:	personal_part last_name suffix_part EOLTOKEN
		|	personal_part last_name EOLTOKEN
		|	error EOLTOKEN { fprintf(stdout, "Bad name-part...skipping to newline\n"); } 
	 	;

personal_part	:	NAMETOKEN { fprintf(stderr, "<FirstName>%s</FirstName>\n", $1); }
		|	NAME_INITIAL_TOKEN { fprintf(stderr, "<FirstName>%s</FirstName>\n", $1); }
	     	;

last_name	:	NAMETOKEN { fprintf(stderr, "<LastName>%s</LastName>\n", $1); }
	 	;

suffix_part	:	SRTOKEN { fprintf(stderr, "<Suffix>Sr.</Suffix>\n"); }
		|	JRTOKEN { fprintf(stderr, "<Suffix>Jr.</Suffix>\n"); }
		|	ROMANTOKEN { fprintf(stderr, "<Suffix>%s</Suffix>\n", $1); }
	   	;

street_address	:	street_number street_name INTTOKEN EOLTOKEN { fprintf(stderr, "<AptNum>%d</AptNum>\n", $3); }
		| 	street_number street_name HASHTOKEN INTTOKEN EOLTOKEN { fprintf(stderr, "<AptNum>%d</AptNum>\n", $4); }
	      	| 	street_number street_name EOLTOKEN
		| 	error EOLTOKEN { fprintf(stdout, "Bad address_line...skipping to newline\n"); }
	      	;

street_number	:	INTTOKEN { fprintf(stderr, "<HouseNumber>%d</HouseNumber>\n", $1); }
		|	IDENTIFIERTOKEN { fprintf(stderr, "<HouseNumber>%s</HouseNumber>\n", $1); }
	     	;

street_name	:	NAMETOKEN { fprintf(stderr, "<StreetName>%s</StreetName>\n", $1); }
		;

location_part	:	town_name COMMATOKEN state_code zip_code EOLTOKEN
		|	error EOLTOKEN { fprintf(stdout, "Bad location_line...skipping to newline\n"); }
	   	;

town_name	:	NAMETOKEN { fprintf(stderr, "<City>%s</City>\n", $1); }
	 	;

state_code	:	NAMETOKEN { fprintf(stderr, "<State>%s</State>\n", $1); }
	  	;

zip_code	:	INTTOKEN DASHTOKEN INTTOKEN { fprintf(stderr, "<Zip5>%d</Zip5>\n", $1); fprintf(stderr, "<Zip4>%d</Zip4>\n", $3); }
		|	INTTOKEN { fprintf(stderr, "<Zip5>%d</Zip5>\n", $1); }
		;
%%

/* 
 * This section is empty on purpose.
 *    DO NOT PUT YOUR main() routine here.
 *    
 * Put your main routine in a separate file.
 */

void yyerror(char *s)
{
  /*
  printf("Syntax Error On Line %s\n", s);
  return 0;
  */
}