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
  char *colorname;
  struct ast_node *node;
}


%token <value>    VALUE       "value"
%token <name>     NAME        "name"
%token <colorname> COLORNAME  "colorname"

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

/* tokens relatifs aux caractères spéciaux */
%token            KW_COMMA ","


%left '+' '-'
%left '*' '/'

%type <node> unit cmds cmd expr block

%%

unit:
    cmds              { $$ = $1; ret->unit = $$; }
;

block:
  KW_BRACK_LEFT cmds KW_BRACK_RIGHT {$$ = make_expr_block($2);}
  | cmd {$$ = make_expr_block($1);}
;

cmds:
    cmd cmds          { $1->next = $2; $$ = $1; }
  | /* empty */       { $$ = NULL;/* end of the process */}   
;

cmd:
    KW_PRINT expr     { $$ = make_cmd_simple(CMD_PRINT, $2);}
  | KW_UP             { $$ = make_cmd_no_arg(CMD_UP);}
  | KW_DOWN           { $$ = make_cmd_no_arg(CMD_DOWN);}
  | KW_FORWARD expr   { $$ = make_cmd_simple(CMD_FORWARD, $2);}
  | KW_BACKWARD expr  { $$ = make_cmd_simple(CMD_BACKWARD, $2);}
  | KW_POSITION expr KW_COMMA expr  { $$ = make_cmd_pos($2,$4);}
  | KW_RIGHT expr     { $$ = make_cmd_simple(CMD_RIGHT, $2);}
  | KW_LEFT expr      { $$ = make_cmd_simple(CMD_LEFT, $2);}
  | KW_HEADING expr   { $$ = make_cmd_simple(CMD_HEADING, $2);}
  | KW_COLOR COLORNAME { $$ = make_cmd_color($2); }
  | KW_COLOR expr KW_COMMA expr KW_COMMA expr {$$ = make_cmd_color_3_args($2,$4,$6);}
  | KW_HOME           { $$ = make_cmd_no_arg(CMD_HOME);}
  | KW_SET NAME expr  { $$ = make_cmd_set($2,$3);}
  | KW_PROC NAME block { $$ = make_cmd_proc($2,$3);}
  | KW_CALL NAME      { $$ = make_cmd_call($2);}
  | KW_REPEAT expr block    { $$ = make_cmd_repeat($2,$3);}
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
  |  KW_RANDOM KW_PARENTH_LEFT expr KW_COMMA expr KW_PARENTH_RIGHT {$$ = make_expr_rand($3,$5);}
  |  KW_SQRT expr {$$ = make_expr_func(FUNC_SQRT, $2);}
  |  NAME {$$ = make_expr_name($1);}
  |  VALUE {$$ = make_expr_value($1);}
  
;

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
