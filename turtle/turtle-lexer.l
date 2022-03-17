%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"

//Merci la L2

/*
 * Get the size of the string.
 */
size_t str_length(const char *str) {
	if(str==NULL){
		return 0;
	}
	size_t size = 0;
	while (str[size]!='\0'){
		++size;
	}
	return size;
}

/*
 * Duplicate a string.
 * Returns a copy of the argument, allocated with malloc or calloc.
 */
char *str_duplicate(const char *str){
    size_t size = str_length(str);
    char* replika = malloc(size*sizeof(char)+1);
    if(replika!=NULL && str!=NULL){
        for(size_t i = 0; i<=size; ++i){
            *(replika+i)=*(str+i);
        }
    }
    return replika;
}

%}

%option warn 8bit nodefault noyywrap

DIGIT [0-9]
NUMBER {DIGIT}+
DECIMALNUMBER {NUMBER}\.{NUMBER}|{NUMBER}
VAR [A-Z][A-Z0123456789]*
COLORNAME red|green|blue|cyan|magenta|yellow|black|gray|white

%%

 /* reconnaissance des tokens relatifs aux commandes */

"up"                  {return KW_UP;}
"down"                {return KW_DOWN;}
forward|fw        {return KW_FORWARD;}
backward|bw       {return KW_BACKWARD;}
right|rt          {return KW_RIGHT;}
left|lt           {return KW_LEFT;}
heading|hd        {return KW_HEADING;}
position|pos      {return KW_POSITION;}
"home"                {return KW_HOME;}
"color"               {return KW_COLOR;}
position|pos      {return KW_POSITION;}
"print"               {return KW_PRINT;}
"set"                 {return KW_SET;}
"proc"                {return KW_PROC;}
"call"                {return KW_CALL;}
"repeat"              {return KW_REPEAT;}

 /* reconnaissance des tokens relatifs aux fonctions */

"sin"               {return KW_SIN;}
"cos"               {return KW_COS;}
"tan"               {return KW_TAN;}
"sqrt"              {return KW_SQRT;}
"random"            {return KW_RANDOM;}

 /* reconnaissance des tokens relatifs aux caractères spéciaux*/

"-"                   {return KW_MINUS;}
"+"                   {return KW_PLUS;}
"*"                   {return KW_TIMES;}
"^"                   {return KW_EXP;}
"("                   {return KW_PARENTH_LEFT;}
")"                   {return KW_PARENTH_RIGHT;}
"{"                   {return KW_BRACK_LEFT;}
"}"                   {return KW_BRACK_RIGHT;}
","                   {return KW_COMMA;}



{COLORNAME}           {yylval.colorname = str_duplicate(yytext); return COLORNAME;}
{DECIMALNUMBER}       {yylval.value = strtod(yytext, NULL); return VALUE; }

 /* Variables et procédures */
{VAR}                 {yylval.name = str_duplicate(yytext);return NAME;}

 /* lignes vides, commentaires et tabulations */
#.*             /*ignore*/
[\n\t ]*          /*ignore*/

 /* Caractères non reconnus */
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
