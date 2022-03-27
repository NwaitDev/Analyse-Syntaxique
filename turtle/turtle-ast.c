#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "array.h"
#include <math.h>


//definitions of the default constants for each context
#define PI 3.141592653589793
#define SQRT2 1.41421356237309504880
#define SQRT3 1.7320508075688772935


//converts angles from degrees to radians
double angle(double deg){
  return 2*PI * deg/360;
}


struct ast_node *make_cmd_no_arg(enum ast_cmd cmd){
  assert(cmd==CMD_UP||cmd==CMD_DOWN||cmd==CMD_HOME);
  struct ast_node *node = calloc(1,sizeof(struct ast_node)); //allocationg memory for the node
  node->kind = KIND_CMD_SIMPLE; //defining the type of the command
  node->u.cmd = cmd; //defining th name of the command in the node
  node->children_count = 0; // no argument for command so no child
  node->next = NULL; //making sure the command doesn't have a following node by default
  return node;
}


struct ast_node *make_cmd_simple(enum ast_cmd cmd, struct ast_node *arg){
  assert(arg->kind == KIND_EXPR_BINOP ||
         arg->kind == KIND_EXPR_UNOP ||
         arg->kind == KIND_EXPR_FUNC ||
         arg->kind == KIND_EXPR_VALUE ||
         arg->kind == KIND_EXPR_NAME);
  struct ast_node *node = calloc(1,sizeof(struct ast_node));//allocationg memory for the node
  node->kind = KIND_CMD_SIMPLE; //defining the type of the command
  node->u.cmd = cmd; //defining th name of the command in the node
  node->children_count = 1; // one argument for the command so one child
  node->children[0] = arg; //defining children as the argument node
  node->next = NULL; //making sure the command doesn't have a following node by default
  return node;
}



struct ast_node *make_cmd_call(char *proc_to_call){
  struct ast_node *node = calloc(1,sizeof(struct ast_node));//allocationg memory for the node
  node->kind = KIND_CMD_CALL; //defining the type of the command
  node->u.name = proc_to_call; //defining th name of the command to call with this node
  node->children_count = 0; // no need for a child as the procedure to call is saved in the context
  node->next = NULL; //making sure the command doesn't have a following node by default
  return node;
}

/*
Manage colors separatly because the parameter COLORNAME is
not something that is represented by an expression (double) 
but a string
KW_COLOR COLORNAME
*/
struct ast_node *make_cmd_color(char* color){
  struct ast_node *node = calloc(1,sizeof(struct ast_node)); //allocating the memory
  node->kind = KIND_CMD_SIMPLE; //defining the command kind
  node->u.cmd = CMD_COLOR; //defining the command type
  //even though the argument is just a string, 
  //we will store this value as if it was declared as a 3 values color
  node->children_count = 3; 
  node->next = NULL;

  //for now, we store the values following the color given in argument
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



struct ast_node *make_cmd_pos(struct ast_node *arg1, struct ast_node *arg2){
  struct ast_node *node = calloc(1,sizeof(struct ast_node)); //allocating the memory for the node
  node->kind = KIND_CMD_SIMPLE; //defining the kind
  node->u.cmd = CMD_POSITION; //defining the type of cmd
  node->children_count = 2; // 2 children : x position and y position
  node->children[0] = arg1;
  node->children[1] = arg2;
  node->next = NULL;
  return node;
}

struct ast_node *make_cmd_set(char* name, struct ast_node *expr){
  struct ast_node *node = calloc(1,sizeof(struct ast_node)); //allocating memory for the node
  node->kind = KIND_CMD_SET; //defining the cmd kind
  node->u.name = name; //defining the name of the variable to set
  node->children_count = 1; //the child is the expression representing the variable to set
  node->children[0] = expr;
  node->next = NULL;
  return node;
}


struct ast_node *make_cmd_proc(char* name, struct ast_node *bloc){
  struct ast_node *node = calloc(1,sizeof(struct ast_node)); //alloc memory
  node->kind = KIND_CMD_PROC; //def kind
  node->u.name = name; //defining the name of the cmd block to use as a procedure
  node->children_count = 1; //the child is the cmd block
  node->children[0] = bloc;
  node->next = NULL;
  return node;
}


struct ast_node *make_cmd_repeat(struct ast_node *expr, struct ast_node *block){
  struct ast_node *node = calloc(1,sizeof(struct ast_node)); //alloc memory
  node->kind = KIND_CMD_REPEAT; //cmd kind
  node->u.name = "repeat";
  node->children_count = 2; //the children are :
  node->children[0] = expr; // the nb of times to repeat the block
  node->children[1] = block; //the block to repeat
  node->next = NULL;
  return node;
}


struct ast_node *make_cmd_color_3_args(struct ast_node *red, struct ast_node *green,struct ast_node *blue){
  struct ast_node *node = calloc(1,sizeof(struct ast_node)); //alloc mem
  node->kind = KIND_CMD_SIMPLE; //cmd kind
  node->u.cmd = CMD_COLOR; //cmd type
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
  node->children[0]=arg1; //corresponds to the lower bound of the interval in which the random number is chosen
  node->children[1]=arg2; //corresponds to the upper bound of the interval in which the random number is chosen
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
  node->u.name = "block";
  node->children_count = 1;
  node->children[0] = cmds;
  node->next = NULL;
  return node;
}


//Destroys a node and all its children recursively and its siblings too
void ast_node_destroy(struct ast_node *self){
  struct ast_node* node = self;
  //destroying children
  for(size_t i = 0; i<node->children_count;++i){
    ast_node_destroy(node->children[i]);
  }

  //destroying siblings
  if(node->next==NULL){
    free(self);
  }else{
    ast_node_destroy(node->next);
    node->next=NULL;
    free(self);
  }
}

void ast_destroy(struct ast *self) {
  ast_node_destroy(self->unit);
}

//creates a context for the program to be parsed
void context_create(struct context *self){
  //initial position and angle for the turtle
  self->angle = 0;
  self->up = false;
  self->x = 0;
  self->y = 0;
  //creating the structures to hold variables and procedures
  self->procs = init_tab();
  self->vars = init_tab();

  //adding default constants to the programs
  struct ast_node* pi = make_cmd_set("PI",make_expr_value(PI));
  struct ast_node* sqrt2 = make_cmd_set("SQRT2",make_expr_value(SQRT2));
  struct ast_node* sqrt3 = make_cmd_set("SQRT3",make_expr_value(SQRT3));
  append_node(self->vars,pi);
  append_node(self->vars,sqrt2);
  append_node(self->vars,sqrt3);
}

//frees up the space taken by the context
void context_destroy(struct context *self){
  destroy_tab(self->procs);
  destroy_tab(self->vars);
}


//Function that evaluates a node and its children recursively
struct ast_node* node_eval(struct ast_node* node, struct context *ctx){
  double potential_val, potential_val2; //while evaluating, we'll eventually need to save up to two values to correctly evaluate things
  struct ast_node* current = node; //while evaluating, we'll eventually need to use an auxiliary node (the previously saved procedures for example)

  //switch on the command kind to determine which action to do 
  switch(node->kind){

    case KIND_CMD_SIMPLE:
      switch (node->u.cmd){
        //Commands with no arguments
        case CMD_UP: //raises up the pen
          ctx->up = true; 
          break;
        case CMD_DOWN: //put down the pen
          ctx->up = false;
          break;
        case CMD_HOME: //returning the turtle to the initial position
          ctx->up = false;
          ctx->x = 0;
          ctx->y = 0;
          ctx->r = 0;
          ctx->g = 0;
          ctx->b = 0;
          ctx->angle = 0;
          break; 
        
        
        //Commands with one argument of type expr
        case CMD_PRINT: //printing on standard erro the value of the node
          node->children[0] = node_eval(node->children[0],ctx);
          potential_val = node->children[0]->u.value;
          fprintf(stderr,"%f\n",potential_val);
          break;

        case CMD_FORWARD : //moving forward 
          node->children[0] = node_eval(node->children[0],ctx);
          potential_val = node->children[0]->u.value; //distance = evaluation of the parameter
          ctx->x = ctx->x + cos(ctx->angle)*potential_val; //taking into account the direction of the turtle
          ctx->y = ctx->y + sin(ctx->angle)*potential_val;
          if(ctx->up){
            fprintf(stdout,"MoveTo %f %f\n",ctx->x,ctx->y); //if the pen's up, just moving
          }else{
            fprintf(stdout,"LineTo %f %f\n",ctx->x,ctx->y); //if the pen's down, drawing a line
          }
          break;

        case CMD_BACKWARD : //same as forward but in the other direction
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

        case CMD_RIGHT : //turning the direction of the turle
          node->children[0] = node_eval(node->children[0],ctx);
          potential_val = node->children[0]->u.value;
          ctx->angle += angle(potential_val);
          break;

        case CMD_LEFT : //turning the direction of the turle
          node->children[0] = node_eval(node->children[0],ctx);
          potential_val = node->children[0]->u.value;
          ctx->angle -= angle(potential_val); //do not forget to convert the angle to radians
          break;

        case CMD_HEADING: //setting the direction of the turle
          node->children[0] = node_eval(node->children[0],ctx);
          ctx->angle = angle(node->children[0]->u.value);
          break;

        //2 parameters command
        case CMD_POSITION: //setting the position of the turle
          node->children[0] = node_eval(node->children[0],ctx);
          node->children[1] = node_eval(node->children[1],ctx);
          ctx->x = node->children[0]->u.value;
          ctx->y = node->children[1]->u.value;
          fprintf(stdout,"MoveTo %f %f\n",ctx->x,ctx->y);
          break;

        //3 parameters command
        case CMD_COLOR: //setting the color following the red, green and blue children of the node
          node->children[0] = node_eval(node->children[0],ctx);
          node->children[1] = node_eval(node->children[1],ctx);
          ctx->r = node->children[0]->u.value;
          ctx->g = node->children[1]->u.value;
          ctx->b = node->children[2]->u.value;
          fprintf(stdout,"Color %f %f %f\n",ctx->r,ctx->g,ctx->b);
          break;
      }
      break;

    case KIND_CMD_REPEAT : //evaluating several times the block used as second parameter
      node->children[0] = node_eval(node->children[0],ctx);
      potential_val = node->children[0]->u.value;
      while(potential_val>0){
        node->children[1] = node_eval(node->children[1],ctx);
        potential_val-=1;
      }
      break;

    case KIND_CMD_BLOCK :
      //Undefined lol, not used in the program
      break;

    case KIND_EXPR_BLOCK : //evaluating every node of the list of commands
      current = node->children[0];
      while(current!=NULL){
        node_eval(current, ctx);
        current = current->next;
      }
      break;

    case KIND_CMD_PROC : //adding the current node to the list of procedures of the context
      append_node(ctx->procs,node);
      break;

    case KIND_CMD_SET : //adding the current node to the list of variables
      //evaluate the child node to "convert it into a value" so that there is no need to reevaluate it later
      node->children[0] = node_eval(node->children[0],ctx); 
      append_node(ctx->vars, node);
      break;

    case KIND_CMD_CALL : //looking for the procedure in the proc list of the context. If found, evaluate it
      current = get_node(ctx->procs,node->u.name);
      if(NULL!=current){
        node_eval(current->children[0],ctx);
      }else{
        fprintf(stderr,"error, couldn't find any procedure called %s\n",node->u.name);
      }
      break;
    
    
    /*
      for expressions, we recursively evaluate the children
      every time an expression is evaluated, 
      its u field has automatically a u.value field set 
      to the value of the evaluation of its children.
      This way, we make sure that whatever the kind of expression,
      we will always be able to get its value.
    */

    case KIND_EXPR_NAME : //looking for the variable name in the vars
      current = get_node(ctx->vars,node->u.name);
      if(NULL!=current){
        node->children[0] = node_eval(current,ctx); //get the value of it and making it the node's value
        node->u.value = node->children[0]->u.value; 
      }else{
        fprintf(stderr,"error, couldn't find any variable called %s\n",node->u.name);
      }
      break;

    case KIND_EXPR_FUNC : //for each function, we evaluate the children and set this node's value to the function applied to the result of the children's values
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
      //Nothing to do the value remains a value, the node is already evaluated
      break;
    case KIND_EXPR_UNOP :
      //just need to take the opposite because there only is 1 unary operation
      node->children[0] = node_eval(node->children[0],ctx);
      node->u.value = - node->children[0]->u.value;
      break;
    case KIND_EXPR_BINOP ://same job as the function nodes
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
      break;
  }
  return node;
}

//we evaluate successively all the nodes in the nodes sequence of the ast
void ast_eval(const struct ast *self, struct context *ctx) {
  struct ast_node* current = self->unit;
  while(current!=NULL){
    node_eval(current, ctx);
    current = current->next;
  }
}

/*
 * symbol used for each node when debugging
 */
void ast_symbol(struct ast_node* self){
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
      fprintf(stderr,"repeat");
      break;
    case KIND_CMD_BLOCK :
      fprintf(stderr,"block");
      break;
    case KIND_CMD_PROC :
      fprintf(stderr,"proc ");
      break;
    case KIND_CMD_CALL :
      fprintf(stderr,"call");
      break;
    case KIND_CMD_SET :
      fprintf(stderr,"set");
      break;
    
    case KIND_EXPR_FUNC :
      switch(self->u.func){
        case FUNC_COS:
          fprintf(stderr,"cos");
          break;
        case FUNC_RANDOM :
          fprintf(stderr,"random");
          break;
        case FUNC_SIN :
          fprintf(stderr,"sin");
          break;
        case FUNC_SQRT :
          fprintf(stderr,"sqrt");
          break;
        case FUNC_TAN :
          fprintf(stderr,"tan");
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

//print the nodes symbols with indentation to clearly see which node contains which node
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
    }
  }
}

//prints the ast structure for debugging by printing all its nodes
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
