#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "array.h"
#include <math.h>

#define PI 3.141592653589793
#define SQRT2 1.41421356237309504880
#define SQRT3 1.7320508075688772935

double angle(double deg){
  return 2*PI * deg/360;
}

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
  node->u.cmd = cmd;
  node->children_count = 0;
  node->next = NULL;
  return node;
}

/*
1 argument command
KW_PRINT expr
KW_FORWARD expr
KW_BACKWARD expr   
KW_RIGHT expr     
KW_LEFT expr      
KW_HEADING expr
*/
struct ast_node *make_cmd_simple(enum ast_cmd cmd, struct ast_node *arg){
  assert(arg->kind == KIND_EXPR_BINOP ||
         arg->kind == KIND_EXPR_UNOP ||
         arg->kind == KIND_EXPR_FUNC ||
         arg->kind == KIND_EXPR_VALUE ||
         arg->kind == KIND_EXPR_NAME);
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = cmd;
  node->children_count = 1;
  node->children[0] = arg;
  node->next = NULL;
  return node;
}

/*
1 argument command
KW_CALL NAME
*/
struct ast_node *make_cmd_call(char *proc_to_call){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_CALL;
  node->u.name = proc_to_call;
  node->children_count = 0;
  node->next = NULL;
  return node;
}

/*
Manage colors separatly because the parameter COLORNAME is
not something that is represented by an expression(double) 
but a string
KW_COLOR COLORNAME
*/
struct ast_node *make_cmd_color(char* color){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_COLOR;
  node->children_count = 3;
  node->next = NULL;

  if(strcmp(color,"red")==0){
    node->children[0] = make_expr_value(1.0);
    node->children[1] = make_expr_value(0);
    node->children[2] = make_expr_value(0);
    return node;
  }
  if(strcmp(color,"green")==0){
    node->children[0] = make_expr_value(0);
    node->children[1] = make_expr_value(1.0);
    node->children[2] = make_expr_value(0);
    return node;
  }
  if(strcmp(color,"blue")==0){
    node->children[0] = make_expr_value(0);
    node->children[1] = make_expr_value(0);
    node->children[2] = make_expr_value(1.0);
    return node;
  }
  if(strcmp(color,"cyan")==0){
    node->children[0] = make_expr_value(0);
    node->children[1] = make_expr_value(1.0);
    node->children[2] = make_expr_value(1.0);
    return node;
  }
  if(strcmp(color,"magenta")==0){
    node->children[0] = make_expr_value(1.0);
    node->children[1] = make_expr_value(0);
    node->children[2] = make_expr_value(1.0);
    return node;
  }
  if(strcmp(color,"yellow")==0){
    node->children[0] = make_expr_value(1.0);
    node->children[1] = make_expr_value(1.0);
    node->children[2] = make_expr_value(0);
    return node;
  }
  if(strcmp(color,"black")==0){
    node->children[0] = make_expr_value(0);
    node->children[1] = make_expr_value(0);
    node->children[2] = make_expr_value(0);
    return node;
  }
  if(strcmp(color,"gray")==0){
    node->children[0] = make_expr_value(0.5);
    node->children[1] = make_expr_value(0.5);
    node->children[2] = make_expr_value(0.5);
    return node;
  }
  if(strcmp(color,"white")==0){
    node->children[0] = make_expr_value(1.0);
    node->children[1] = make_expr_value(1.0);
    node->children[2] = make_expr_value(1.0);
    return node;
  }
  fprintf(stderr,"cannot find any color called %s\n",color);
  return node;
}


/*
2 arguments command
KW_POSITION expr expr
*/
struct ast_node *make_cmd_pos(struct ast_node *arg1, struct ast_node *arg2){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_POSITION;
  node->children_count = 2;
  node->children[0] = arg1;
  node->children[1] = arg2;
  node->next = NULL;
  return node;
}

/*
2 arguments command
KW_SET NAME expr
*/
struct ast_node *make_cmd_set(char* name, struct ast_node *expr){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_SET;
  node->u.name = name;
  node->children_count = 1;
  node->children[0] = expr;
  node->next = NULL;
  return node;
}

/*
2 arguments command
KW_PROC NAME block
*/
struct ast_node *make_cmd_proc(char* name, struct ast_node *bloc){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_PROC;
  node->u.name = name;
  node->children_count = 1;
  node->children[0] = bloc;
  node->next = NULL;
  return node;
}

/*
2 arguments command
KW_REPEAT expr block
*/
struct ast_node *make_cmd_repeat(struct ast_node *expr, struct ast_node *block){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_REPEAT;
  node->u.cmd = 0; // undefined behavour 
  node->children_count = 2;
  node->children[0] = expr;
  node->children[1] = block;
  node->next = NULL;
  return node;
}

/*
3 argument command
KW_COLOR expr1 expr2 expr3   
*/
struct ast_node *make_cmd_color_3_args(struct ast_node *red, struct ast_node *green,struct ast_node *blue){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_COLOR; 
  node->children_count = 3;
  node->children[0] = red;
  node->children[1] = green;
  node->children[2] = blue;
  node->next = NULL;
  return node;
}

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
  node->next = NULL;
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
  node->next = NULL;
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
  node->next = NULL;
  return node;
}

/*
Expression as a function applied to an expression
  cos sin tan
*/
struct ast_node *make_expr_func(enum ast_func fun, struct ast_node* arg){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_EXPR_FUNC;
  node->u.func = fun;
  node->children_count = 1;
  node->children[0]=arg;
  node->next = NULL;
  return node;
}

/*
Expression as a function applied to an expression
  random
*/
struct ast_node *make_expr_rand(struct ast_node* arg1, struct ast_node* arg2){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_RANDOM;
  node->children_count = 2;
  node->children[0]=arg1;
  node->children[0]=arg2;
  node->next = NULL;
  return node;
}

/*
Expression as a value
*/
struct ast_node *make_expr_value(double value) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_VALUE;
  node->u.value = value;
  node->next = NULL;
  return node;
}

/*
Expression that is used to give a block as an argument for a command
*/
struct ast_node *make_expr_block(struct ast_node* cmds){
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BLOCK;
  node->u.cmd = 0; //undefined behavour
  node->children_count = 1;
  node->children[0] = cmds;
  node->next = NULL;
  return node;
}





//AST create and destroy
void ast_node_destroy(struct ast_node *self){
  for(size_t i = 0; i<self->children_count;++i){
    ast_node_destroy(self->children[i]);
    free(self->children[i]);
  }
  free(self->children);
}

void ast_destroy(struct ast *self) {
  struct ast_node *current = self->unit;
  struct ast_node *next = self->unit->next;
  while(current!=NULL){
    ast_node_destroy(current);
    current = next;
    next = next->next;
  }
}

/*
 * context
 */

void context_create(struct context *self) {
  self=calloc(1,sizeof(struct context));
  self->angle = 0;
  self->up = false;
  self->x = 0;
  self->y = 0;
  self->procs = init_tab();
  self->vars = init_tab();
}

void context_destroy(struct context *self){
  destroy_tab(self->procs);
  destroy_tab(self->vars);
  free(self);
}

/*
 * eval
 */

struct ast_node* node_eval(struct ast_node* node, struct context *ctx){
  double potential_val, potential_val2;
  struct ast_node* current = node;
  switch(node->kind){
    case KIND_CMD_SIMPLE:
      switch (node->u.cmd){
        //Commands with no arguments
        case CMD_UP:
          ctx->up = true;
          break;
        case CMD_DOWN:
          ctx->up = false;
          break;
        case CMD_HOME:
          ctx->up = false;
          ctx->x = 0;
          ctx->y = 0;
          ctx->r = 0;
          ctx->g = 0;
          ctx->b = 0;
          ctx->angle = 0;
          break; 
        
        //Commands with one argument of type expr
        
        case CMD_PRINT:
          node->children[0] = node_eval(node->children[0],ctx);
          potential_val = node->children[0]->u.value;
          fprintf(stderr,"%f\n",potential_val);
          break;

        case CMD_FORWARD :
          node->children[0] = node_eval(node->children[0],ctx);
          potential_val = node->children[0]->u.value;
          ctx->x = ctx->x + cos(ctx->angle)*potential_val;
          ctx->y = ctx->y + sin(ctx->angle)*potential_val;
          if(ctx->up){
            fprintf(stdout,"MoveTo %f %f\n",ctx->x,ctx->y);
          }else{
            fprintf(stdout,"LineTo %f %f\n",ctx->x,ctx->y);
          }
          break;

        case CMD_BACKWARD :
          node->children[0] = node_eval(node->children[0],ctx);
          potential_val = node->children[0]->u.value;
          ctx->x = ctx->x - cos(ctx->angle)*potential_val;
          ctx->y = ctx->y - sin(ctx->angle)*potential_val;
          if(ctx->up){
            fprintf(stdout,"MoveTo %f %f\n",ctx->x,ctx->y);
          }else{
            fprintf(stdout,"LineTo %f %f\n",ctx->x,ctx->y);
          }
          break;
        case CMD_RIGHT :
          node->children[0] = node_eval(node->children[0],ctx);
          potential_val = node->children[0]->u.value;
          ctx->angle += angle(potential_val);
          break;
        case CMD_LEFT :
          node->children[0] = node_eval(node->children[0],ctx);
          potential_val = node->children[0]->u.value;
          ctx->angle += angle(potential_val);
          break;
        case CMD_HEADING:
          node->children[0] = node_eval(node->children[0],ctx);
          ctx->angle = angle(node->children[0]->u.value);
          break;

        //2 parameters command
        case CMD_POSITION:
          node->children[0] = node_eval(node->children[0],ctx);
          node->children[1] = node_eval(node->children[1],ctx);
          ctx->x = node->children[0]->u.value;
          ctx->y = node->children[1]->u.value;
          fprintf(stdout,"MoveTo %f %f\n",ctx->x,ctx->y);
          break;

        //3 parameters command
        case CMD_COLOR:
          node->children[0] = node_eval(node->children[0],ctx);
          node->children[1] = node_eval(node->children[1],ctx);
          ctx->r = node->children[0]->u.value;
          ctx->g = node->children[1]->u.value;
          ctx->b = node->children[2]->u.value;
          fprintf(stdout,"Color %f %f %f\n",ctx->r,ctx->g,ctx->b);
          break;
      }
      break;
    case KIND_CMD_REPEAT :
      node->children[0] = node_eval(node->children[0],ctx);
      potential_val = node->children[0]->u.value;
      while(potential_val>0){
        node->children[1] = node_eval(node->children[1],ctx);
        potential_val-=1;
      }
      break;
    case KIND_CMD_BLOCK :
      //Undefined lol
      break;
    case KIND_EXPR_BLOCK :
      current = node->children[0];
      while(current!=NULL){
        node_eval(current, ctx);
        current = current->next;
      }
      break;
    case KIND_CMD_PROC :
      append_node(ctx->procs,node);
      break;
    case KIND_CMD_SET :
      node->children[0] = node_eval(node->children[0],ctx);
      append_node(ctx->vars, node);
      break;
    case KIND_CMD_CALL :
      if(NULL!=get_node(ctx->procs,node->u.name)){
        node_eval(get_node(ctx->procs,node->u.name),ctx);
      }else{
        fprintf(stderr,"error, couldn't find any procedure called %s\n",node->u.name);
      }
      break;
    case KIND_EXPR_NAME :
      if(NULL!=get_node(ctx->procs,node->u.name)){
        node->children[0] = node_eval(get_node(ctx->vars,node->u.name),ctx);
        node->u.value = node->children[0]->u.value; 
      }else{
        fprintf(stderr,"error, couldn't find any variable called %s\n",node->u.name);
      }
      break;

    case KIND_EXPR_FUNC :
      switch(node->u.func){
        case FUNC_COS:
          node->children[0] = node_eval(node->children[0],ctx);
          node->u.value = cos(angle(node->children[0]->u.value));
          break;
        case FUNC_SIN:
          node->children[0] = node_eval(node->children[0],ctx);
          node->u.value = sin(angle(node->children[0]->u.value));
          break;
        case FUNC_SQRT:
          node->children[0] = node_eval(node->children[0],ctx);
          node->u.value = sqrt(node->children[0]->u.value);
          break;
        case FUNC_RANDOM:
          node->children[0] = node_eval(node->children[0],ctx);
          node->children[1] = node_eval(node->children[1],ctx);
          potential_val = node->children[0]->u.value;
          potential_val2 = node->children[1]->u.value;
          node->u.value = potential_val + (potential_val2-potential_val)*rand();
          break;
        case FUNC_TAN:
          node->children[0] = node_eval(node->children[0],ctx);
          node->u.value = tan(angle(node->children[0]->u.value));
          break;
      }
      break;
    case KIND_EXPR_VALUE :
      //Nothing to do the value remains a value
      break;
    case KIND_EXPR_UNOP :
      //just need to take the opposite because there only is 1 unary operation
      node->children[0] = node_eval(node->children[0],ctx);
      node->u.value = - node->children[0]->u.value;
      break;
    case KIND_EXPR_BINOP :
      node->children[0] = node_eval(node->children[0],ctx);
      node->children[1] = node_eval(node->children[1],ctx);
      switch(node->u.op){
        case '+':
          node->u.value = node->children[0]->u.value +node->children[1]->u.value; 
        break;
        case '-':
          node->u.value = node->children[0]->u.value - node->children[1]->u.value; 
        break;
        case '*':
          node->u.value = node->children[0]->u.value * node->children[1]->u.value; 
        break;
        case '/':
          node->u.value = node->children[0]->u.value / node->children[1]->u.value; 
        break;
        case '^':
          node->u.value = pow(node->children[0]->u.value,node->children[1]->u.value); 
        break;
      }
      node->kind = KIND_EXPR_VALUE;
      break;
  }
  return node;
}

void ast_eval(const struct ast *self, struct context *ctx) {
  struct ast_node* current = self->unit;
  while(current!=NULL){
    node_eval(current, ctx);
    current = current->next;
  }
}

/*
 * print
 */

void ast_symbol(const struct ast_node* self){
  switch (self->kind){
    case KIND_CMD_SIMPLE :
      switch(self->u.cmd){
        case CMD_UP :
          fprintf(stderr,"up");
          break;
        case CMD_DOWN :
          fprintf(stderr,"down");
          break;
        case CMD_RIGHT :
          fprintf(stderr,"right");
          break;
        case CMD_LEFT :
          fprintf(stderr,"left");
          break;
        case CMD_HEADING :
          fprintf(stderr,"heading");
          break;
        case CMD_FORWARD :
          fprintf(stderr,"forward");
          break;
        case CMD_BACKWARD :
          fprintf(stderr,"backward");
          break;
        case CMD_POSITION :
          fprintf(stderr,"position");
          break;
        case CMD_HOME :
          fprintf(stderr,"home");
          break;
        case CMD_COLOR :
          fprintf(stderr,"color");
          break;
        case CMD_PRINT :
          fprintf(stderr,"print");
          break;
      }
      break;
    case KIND_CMD_REPEAT :
      fprintf(stderr,"repeat ");
      break;
    case KIND_CMD_BLOCK :
      fprintf(stderr,"block ");
      break;
    case KIND_CMD_PROC :
      fprintf(stderr,"proc ");
      break;
    case KIND_CMD_CALL :
      fprintf(stderr,"call ");
      break;
    case KIND_CMD_SET :
      fprintf(stderr,"set ");
      break;
    
    case KIND_EXPR_FUNC :
      switch(self->u.func){
        case FUNC_COS:
          fprintf(stderr,"cos ");
          break;
        case FUNC_RANDOM :
          fprintf(stderr,"random ");
          break;
        case FUNC_SIN :
          fprintf(stderr,"sin ");
          break;
        case FUNC_SQRT :
          fprintf(stderr,"sqrt ");
          break;
        case FUNC_TAN :
          fprintf(stderr,"tan ");
          break;
      }
      break;
    case KIND_EXPR_VALUE :
      fprintf(stderr,"%f ",self->u.value);
      break;
    case KIND_EXPR_UNOP :
      fprintf(stderr,"-");
      break;
    case KIND_EXPR_BINOP :
      fprintf(stderr,"%c ",self->u.op);
      break;
    case KIND_EXPR_NAME :
      fprintf(stderr,"%s",self->u.name);
      break;
    case KIND_EXPR_BLOCK :
      fprintf(stderr,"block ");
      break;
    default:
      fprintf(stderr,"whoops forgot to handle this one ");
  }
}

void ast_node_print(struct ast_node* self,size_t indent){
  struct ast_node * current = self;
  struct ast_node * next;
  while(true){
    next = current->next;
    for(size_t indentation = 0; indentation < indent; ++indentation){
      fprintf(stderr, "\t");
    }
    ast_symbol(current);
    fprintf(stderr,"\n");
    for (size_t child=0; child < current->children_count;++child){
      ast_node_print(current->children[child], indent+1);
    }
    current = next;
    if(next==NULL){
      break;
    }else{
      //TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
    }
  }
}

void ast_print(const struct ast *self) {
  fprintf(stderr,"------------------------\n");
  struct ast_node *current = self->unit;
  struct ast_node *next = self->unit->next;
  while(true){
    ast_node_print(current,0);
    current = next;
    if(current==NULL){
      break;
    }
    next = next->next;
  }
  fprintf(stderr,"------------------------\n");
}
