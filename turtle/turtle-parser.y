%{
#include <stdio.h>

#include "turtle-ast.h"

int yylex();
void yyerror(struct ast *ret, const char *);

%}

%debug
%defines

%define parse.error verbose

%parse-param { struct ast *ret }

%union {
  double value;
  char *name;
  struct ast_node *node;
}

%token <value>    VALUE       "value"
%token <name>     NAME        "name"

/* tokens relatifs aux commandes */
%token            KW_FORWARD  "forward"
%token            KW_BACKWARD  "backward"
%token            KW_UP  "up"
%token            KW_DOWN  "down"
%token            KW_RIGHT  "right"
%token            KW_LEFT  "left"
%token            KW_HEADING  "heading"
%token            KW_POSITION  "position"
%token            KW_HOME  "home"
%token            KW_COLOR  "color"
%token            KW_PRINT  "print"


/* tokens relatifs aux fonctions */
%token            KW_SIN  "sin"
%token            KW_COS  "cos"
%token            KW_TAN  "tan"
%token            KW_SQRT  "sqrt"
%token            KW_RANDOM  "random"

/* tokens relatifs aux autres types de mots clefs */
%token            KW_REPEAT  "repeat"
%token            KW_BLOCK  "block"
%token            KW_PROC  "proc"
%token            KW_CALL  "call"
%token            KW_SET  "set"


/* tokens relatifs aux opérateurs */
%token            KW_PLUS "+"
%token            KW_MINUS "-"
%token            KW_TIMES "*"
%token            KW_EXP "^"
%token            KW_DIV "/"
%token            KW_PARENTH_LEFT "("
%token            KW_PARENTH_RIGHT ")"
%token            KW_BRACK_LEFT "{"
%token            KW_BRACK_RIGHT "}"


%type <node> unit cmds cmd expr 

%%

unit:
    cmds              { $$ = $1; ret->unit = $$; }
;

cmds:
    cmd cmds          { $1->next = $2; $$ = $1; }
  | /* empty */       { $$ = NULL; /* end of the process */}   
;

cmd:
    KW_PRINT expr     { }
  | KW_UP             { /* TODO */}
  | KW_DOWN           { /* TODO */}
  | KW_FORWARD expr   { /* TODO */}
  | KW_BACKWARD expr  { /* TODO */}
  | KW_POSITION expr  { /* TODO */}
  | KW_RIGHT expr     { /* TODO */}
  | KW_LEFT expr      { /* TODO */}
  | KW_HEADING expr   { /* TODO */}
  | KW_COLOR expr     { /* TODO */}
  | KW_HOME           { /* TODO */}
  | KW_SET NAME expr  { /* TODO */}
  | KW_PROC NAME cmds { /* TODO */}
  | KW_CALL NAME      { /* TODO */}
  | KW_REPEAT cmds    { /* TODO */}
;

expr:
    expr KW_PLUS expr {$$ = make_expr_binop('+',$1,$3);}
  |  expr KW_MINUS expr {$$ = make_expr_binop('-',$1,$3);}
  |  expr KW_TIMES expr {$$ = make_expr_binop('*',$1,$3);}
  |  expr KW_EXP expr {$$ = make_expr_binop('^',$1,$3);}
  |  expr KW_DIV expr {$$ = make_expr_binop('/',$1,$3);}
  |  KW_MINUS expr {$$ = make_expr_unnop('-',$2);}
  |  KW_SIN expr {$$ = make_expr_func(FUNC_SIN, $2);}
  |  KW_COS expr {$$ = make_expr_func(FUNC_COS, $2);}
  |  KW_TAN expr {$$ = make_expr_func(FUNC_TAN, $2);}
  |  KW_RANDOM expr {$$ = make_expr_func(FUNC_RANDOM, $2);}
  |  KW_SQRT expr {$$ = make_expr_func(FUNC_SQRT, $2);}
  |  NAME {$$ = make_expr_name($1);}
  |  VALUE {$$ = make_expr_value($1);}
  
;

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
