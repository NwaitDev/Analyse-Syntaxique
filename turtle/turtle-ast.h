#ifndef TURTLE_AST_H
#define TURTLE_AST_H

#include <stddef.h>
#include <stdbool.h>

// simple commands
enum ast_cmd {
  CMD_UP,
  CMD_DOWN,
  CMD_RIGHT,
  CMD_LEFT,
  CMD_HEADING,
  CMD_FORWARD,
  CMD_BACKWARD,
  CMD_POSITION,
  CMD_HOME,
  CMD_COLOR,
  CMD_PRINT,
};

// internal functions
enum ast_func {
  FUNC_COS,
  FUNC_RANDOM,
  FUNC_SIN,
  FUNC_SQRT,
  FUNC_TAN,
};

// kind of a node in the abstract syntax tree
enum ast_kind {
  KIND_CMD_SIMPLE, //up down rt lt hd bw fw pos home color print
  KIND_CMD_REPEAT,
  KIND_CMD_BLOCK,
  KIND_CMD_PROC,
  KIND_CMD_CALL,
  KIND_CMD_SET,

  KIND_EXPR_FUNC,
  KIND_EXPR_VALUE,
  KIND_EXPR_UNOP,
  KIND_EXPR_BINOP,
  KIND_EXPR_BLOCK,
  KIND_EXPR_NAME,
};

#define AST_CHILDREN_MAX 3

// a node in the abstract syntax tree
struct ast_node {
  enum ast_kind kind; // kind of the node

  union {
    enum ast_cmd cmd;   // kind == KIND_CMD_SIMPLE
    double value;       // kind == KIND_EXPR_VALUE, for literals
    char op;            // kind == KIND_EXPR_BINOP or kind == KIND_EXPR_UNOP, for operators in expressions
    char *name;         // kind == KIND_EXPR_NAME, the name of procedures and variables
    enum ast_func func; // kind == KIND_EXPR_FUNC, a function
  } u;

  size_t children_count;  // the number of children of the node
  struct ast_node *children[AST_CHILDREN_MAX];  // the children of the node (arguments of commands, etc)
  struct ast_node *next;  // the next node in the sequence
};

// root of the abstract syntax tree
struct ast {
  struct ast_node *unit;
};

// the execution context
struct context {
  double x;
  double y;
  double angle;
  bool up;
  double r, g, b;

  struct dynamic_tab* procs;
  struct dynamic_tab* vars;
};

/*
0 argument command
KW_UP
KW_DOWN
KW_HOME
Function that creates a node representing a node for a command with no argument
*/
struct ast_node *make_cmd_no_arg(enum ast_cmd cmd);

/*
1 argument command
KW_PRINT expr
KW_FORWARD expr
KW_BACKWARD expr  
KW_POSITION expr  
KW_RIGHT expr     
KW_LEFT expr      
KW_HEADING expr
Function that creates a node representing a node for a command with one argument of type expression
*/
struct ast_node *make_cmd_simple(enum ast_cmd cmd, struct ast_node *arg);

/*
Function that creates a node representing a node for the call command
*/
struct ast_node *make_cmd_call(char *proc_to_call);

/*
Manage colors separatly because the parameter COLORNAME is
not something that is represented by an expression(double) 
but by a string
KW_COLOR COLORNAME
Function that creates a node representing a color thank to the name of the color
*/
struct ast_node *make_cmd_color(char* color);



//2 arguments commands :

/*
KW_POSITION expr expr
Function that creates a node corresponding to a position setting
*/
struct ast_node *make_cmd_pos(struct ast_node *arg1, struct ast_node *arg2);

/*
KW_SET NAME expr
function that creates a node corresponding to the set command
-> sets the value of the variable NAME to the value of expr
*/
struct ast_node *make_cmd_set(char* name, struct ast_node *arg);

/*
KW_PROC NAME block
Function that creates a node corresponding to the command proc
-> sets the call to NAME to the execution of the command block 
*/
struct ast_node *make_cmd_proc(char* name, struct ast_node *arg);

/*
KW_REPEAT expr block
Function that creates a node corresponding to the repetition expr times 
of the execution of the command block 
*/
struct ast_node *make_cmd_repeat(struct ast_node *expr, struct ast_node *block);


//3 argument command

/*
KW_COLOR expr1 expr2 expr3
Creates a node corresponding to a color which components in r, g and b are expr1, expr2 and expr3 respectively
*/
struct ast_node *make_cmd_color_3_args(struct ast_node *red, struct ast_node *green,struct ast_node *blue);

/*
Binary expressions :
  + - * / ^
  Creates a node corresponding to a a binary expression
*/
struct ast_node *make_expr_binop(char op, struct ast_node* arg1, struct ast_node* arg2);

/*
Unary expression :
  -Creates a node corresponding to a unary expression
*/
struct ast_node *make_expr_unnop(char op, struct ast_node* expr);

/*
Expression in the form of a name
  Creates a node representing a name of a variable
*/
struct ast_node *make_expr_name(char* name);

/*
Expression as a function applied to an expression
  cos sin tan random
*/
struct ast_node *make_expr_func(enum ast_func fun, struct ast_node* arg);

/*
Expression as a function applied to an expression
   random
*/
struct ast_node *make_expr_rand(struct ast_node* arg1, struct ast_node* arg2);

/*
Expression as a value
*/
struct ast_node *make_expr_value(double value);

/*
  Expression that is used to represent a command block passed as argument of a command
*/
struct ast_node *make_expr_block(struct ast_node* expr);

/*
function that prints a node and its children recursively
it indents the printings to see the branches of the ast
*/
void ast_node_print(struct ast_node* self,size_t indent);

/*
prints the symbol corresponding to a node of the ast
*/
void ast_symbol(struct ast_node* self);


/*
Frees up space taken by the ast
*/
void ast_destroy(struct ast *self);

/*
Frees up space taken by the ast_node and its children
*/
void ast_node_destroy(struct ast_node *self);

/*
Creates a context to save the position, the variable names, the procedure names and the angles
*/
void context_create(struct context *self);

/*
frees up space taken by the context
*/
void context_destroy(struct context *self);

// print the tree as if it was a Turtle program
void ast_print(const struct ast *self);

// evaluate the tree and generate some basic primitives
void ast_eval(const struct ast *self, struct context *ctx);

#endif /* TURTLE_AST_H */
