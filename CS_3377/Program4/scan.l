/*
 * Filename:		scan.l
 * Date:		04/03/2020
 * Author:	        Sanjeev Penupala
 * Email:		sxp170022@utdallas.edu
 * Version:		1.0
 * Copyright:		2020, All Rights Reserved
 *
 * Description:
 *
 *	Lex Program 
 */

%option noyywrap

/* C++ Program In Lex File */
%{
  #include "y.tab.h" 
%}

%%

[ \t\r]+			                     { /* eat up whitespace */ }
","                                { return COMMATOKEN; }
"-"                                { return DASHTOKEN; }
"\n"                               { return EOLTOKEN; }
"#"                                { return HASHTOKEN; }
"Jr."                              { return JRTOKEN; }
"Sr."                              { return SRTOKEN; }
([0-9]+[a-zA-Z]+|[a-zA-Z]+[0-9]+)  { yylval.str = yytext;	return IDENTIFIERTOKEN; }
[0-9]+                             { yylval.num = atoi(yytext); return INTTOKEN; }
[MDCLXVI]+                         { yylval.str = yytext; 	return ROMANTOKEN; }
[a-zA-Z]{2,}                       { yylval.str = yytext; 	return NAMETOKEN; }
[a-zA-Z]\.?                        { yylval.str = yytext; 	return NAME_INITIAL_TOKEN; }

%%

/* 
 * This section is empty on purpose.
 *    DO NOT PUT YOUR main() routine here.
 *    
 * Put your main routine in a separate file.
 */