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
  KIND_CMD_SIMPLE,
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

  // TODO: add procedure handling
  // TODO: add variable handling
};

/*
0 argument command
KW_UP
KW_DOWN
KW_HOME
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
KW_CALL NAME
KW_COLOR COLORNAME
*/
struct ast_node *make_cmd_simple(enum ast_cmd cmd, struct ast_node *arg);

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
struct ast_node *make_expr_binop(char op, struct ast_node* arg1, struct ast_node* arg2);

/*
Unary expression :
  -
*/
struct ast_node *make_expr_unnop(char op, struct ast_node* expr);

/*
Expression in the form of a name
*/
struct ast_node *make_expr_name(char* name);

/*
Expression as a function applied to an expression
  cos sin tan random
*/
struct ast_node *make_expr_func(enum ast_func fun, struct ast_node* arg);

/*
Expression as a value
*/
struct ast_node *make_expr_value(double value);

/*
Expression that is used to count nb of loops in repeat
*/
struct ast_node *make_expr_block(struct ast_node* expr);


void ast_destroy(struct ast *self);

void ast_node_destroy(struct ast_node *self);

void context_create(struct context *self);
void context_destroy(struct context *self);

// print the tree as if it was a Turtle program
void ast_print(const struct ast *self);

// evaluate the tree and generate some basic primitives
void ast_eval(const struct ast *self, struct context *ctx);
#endif /* TURTLE_AST_H */
