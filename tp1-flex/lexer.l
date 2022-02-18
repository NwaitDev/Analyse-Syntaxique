%{
#include <stdio.h>
#include <stdlib.h>
%}

%option warn 8bit nodefault noyywrap

BOOLEAN  true|false
DIGIT    [0-9]
NUMBER   [1-9]{DIGIT}*|0
HEXNUM   [1-9a-fA-F]|{NUMBER}
HEX      0[Xx]{HEXNUM}+
FLOAT1   {NUMBER}\.{DIGIT}*
FLOAT2   \.{NUMBER}+


%%

{HEX}               {printf("Hexadecimal Number : %s \n", yytext);}
{NUMBER}            {printf("Natural Number : %s \n", yytext);}
{BOOLEAN}           { printf("Boolean Value: %s\n",yytext);}
{FLOAT1}|{FLOAT2}   {printf("Number : %s \n", yytext);}
[\n\t ]*            /* nothing */
.                   { fprintf(stderr, "Unknown token: '%s'\n", yytext); ; exit(EXIT_FAILURE); }

%%
