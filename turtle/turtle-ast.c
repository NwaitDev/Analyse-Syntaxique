#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592653589793

/*
0 argument command
KW_UP
KW_DOWN
KW_HOME
*/
struct ast_node *make_cmd_no_arg(enum ast_cmd cmd){
  assert(cmd==CMD_UP||cmd==CMD_DOWN||cmd==CMD_HOME);
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.value = cmd;
  node->children_count = 0;
  return node;
}

/*
1 argument command
KW_PRINT expr
KW_FORWARD expr
KW_BACKWARD expr  
KW_POSITION expr  
KW_RIGHT expr     
KW_LEFT expr      
KW_HEADING expr
KW_CALL NAME
KW_COLOR COLORNAME
*/
struct ast_node *make_cmd_simple(enum ast_cmd cmd, struct ast_node *arg){
  assert(arg->kind == KIND_EXPR_BINOP ||
         arg->kind == KIND_EXPR_UNOP ||
         arg->kind == KIND_EXPR_FUNC ||
         arg->kind == KIND_EXPR_VALUE ||
         arg->kind == KIND_EXPR_NAME);
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.value = cmd;
  node->children_count = 1;
  node->children[0] = arg;
  return node;
}

/*
2 arguments command
KW_SET NAME expr
KW_PROC NAME block
KW_REPEAT expr block
*/
struct ast_node *make_cmd_2_args(enum ast_cmd cmd,struct ast_node *arg1, struct ast_node *arg2);

/*
3 argument command
KW_COLOR expr1 expr2 expr3   
*/
struct ast_node *make_cmd_color_3_args(struct ast_node *red, struct ast_node *green,struct ast_node *blue);

/*
Binary expressions :
  + - * / ^
*/
struct ast_node *make_expr_binop(char op, struct ast_node* arg1, struct ast_node* arg2){
  assert(op == '+' || op == '-'|| op == '*' || op == '/' || op =='^');
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_EXPR_BINOP;
  node->u.op = op;
  node->children_count = 2;
  node->children[0] = arg1;
  node->children[1] = arg2;
  return node;
}

/*
Unary expression :
  -
*/
struct ast_node *make_expr_unnop(char op, struct ast_node* expr){
  assert(op == '-');
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_EXPR_UNOP;
  node->u.op = op;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}
/*
Expression in the form of a name
*/
struct ast_node *make_expr_name(char* name){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_EXPR_NAME;
  node->u.name = name;
  node->children_count = 0;
  return node;
}

/*
Expression as a function applied to an expression
  cos sin tan random
*/
struct ast_node *make_expr_func(enum ast_func fun, struct ast_node* arg){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_EXPR_FUNC;
  node->u.func = fun;
  node->children_count = 1;
  node->children[0]=arg;
  return node;
}

/*
Expression as a value
*/
struct ast_node *make_expr_value(double value) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_VALUE;
  node->u.value = value;
  return node;
}

/*
Expression that is used to count nb of loops in repeat
*/
struct ast_node *make_expr_block(struct ast_node* expr);





//AST create and destroy
void ast_node_destroy(struct ast_node *self){
  for(size_t i = 0; i<self->children_count;++i){
    ast_node_destroy(self->children[i]);
    free(self->children[i]);
  }
  free(self->children);
}

void ast_destroy(struct ast *self) {
  ast_node_destroy(self->unit);
  free(self->unit);
}

/*
 * context
 */

void context_create(struct context *self) {
  self=calloc(1,sizeof(struct context));
  self->angle = 0;
  self ->up = false;
  self->x = 0;
  self->y = 0;
}

void context_destroy(struct context *self){
  free(self);
}

/*
 * eval
 */

void ast_eval(const struct ast *self, struct context *ctx) {

}

/*
 * print
 */

const char* ast_symbol(const struct ast_node* self){
  switch (self->kind){
    case KIND_CMD_SIMPLE :
      switch(self->u.cmd){
        case CMD_UP :
          return "up ";
        case CMD_DOWN :
          return "down ";
        case CMD_RIGHT :
          return "right ";
        case CMD_LEFT :
          return "left ";
        case CMD_HEADING :
          return "heading ";
        case CMD_FORWARD :
          return "backward ";
        case CMD_BACKWARD :
          return "backward ";
        case CMD_POSITION :
          return "position ";
        case CMD_HOME :
          return "home ";
        case CMD_COLOR :
          return "color ";
        case CMD_PRINT :
          return "print ";
      }
    case KIND_CMD_REPEAT :
      return "repeat ";
    case KIND_CMD_BLOCK :
      return "block ";
    case KIND_CMD_PROC :
      return "proc ";
    case KIND_CMD_CALL :
      return "call ";
    case KIND_CMD_SET :
      return "set ";
    
    case KIND_EXPR_FUNC :
      switch(self->u.func){
        case FUNC_COS:
          return "cos ";
        case FUNC_RANDOM :
          return "random ";
        case FUNC_SIN :
          return "sin ";
        case FUNC_SQRT :
          return "sqrt ";
        case FUNC_TAN :
          return "tan ";
      }
    case KIND_EXPR_VALUE :
      return "nombre ";
    case KIND_EXPR_UNOP :
      return "-";
    case KIND_EXPR_BINOP :
      return "binop ";
    case KIND_EXPR_NAME :
      return self->u.name;
    case KIND_EXPR_BLOCK :
      return "blockinrepeat ";
  }
  return "whoops forgot to handle this one";
}

void ast_node_print(const struct ast_node* self,size_t indent){
  for(size_t indentation = 0; indentation < indent; ++indentation){
    fprintf(stderr, "\t");
  }
  fprintf(stderr,"%s\n",ast_symbol(self));
  for (size_t child=0; child < self->children_count;++child){
    ast_node_print(self->children[child], indent+1);
  }
}

void ast_print(const struct ast *self) {
  ast_node_print(self->unit,0);
}
