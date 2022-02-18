%{
#include <stdio.h>
#include "main.h"

int yylex();
void yyerror(const char *);

//yydebug = 1;

%}

%debug
%defines

%define parse.error verbose

%union {
  double value;
  const char *name;
}

%token <value>    VALUE       "value"
%token <name>     NAME        "name"
%token            QUIT        "quit"


%left '+' '-'
%left '*' '/'
%left '^'
%left UMINUS

%type <value> expr
%type <value> expr2


%%

input:
    /* empty */
  | input line
;

line:
    '\n'
  | expr '\n'         { printf("> %.10g\n", $1); }
  | QUIT              { exit(0); }
;

expr:
    VALUE             { $$ = $1; }
  | NAME              { $$ = constant($1); }
  | '(' expr ')'      { $$ = $2; }
  | expr '+' expr     { $$ = $1 + $3;/* printf("%f+%f\n",$1,$3);*/}
  | expr expr2        { $$ = $1 + $2; }
  | expr '*' expr     { $$ = $1 * $3; }
  | expr '/' expr     { $$ = $1 / $3; }
  | '-' expr %prec UMINUS { $$ = -$2; }
;


%%

void yyerror(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}
