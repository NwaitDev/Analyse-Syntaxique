%{
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
%}

%option warn 8bit nodefault noyywrap

DIGIT [0-9]
ID    [A-Z][A-Z0-9]*


%%

"+"           { return '+'; }
"-"           { return '-'; }
"*"           { return '*'; }
"/"           { return '/'; }
"("           { return '('; }
")"           { return ')'; }
"\n"          { return '\n'; }
"QUIT"        { return "QUIT"; }
"//".*        /*nothing*/

QUIT          { fprintf(stdout, "Ciao !\n"); exit(EXIT_SUCCESS);}
{DIGIT}+      { yylval.value = strtod(yytext, NULL); return VALUE; }
{ID}          { yylval.name = strdup(yytext); return NAME; }
[\t ]*        /* nothing */
.             { fprintf(stderr, "Unknown token: '%s'\n", yytext); ; exit(EXIT_FAILURE); }

%%
