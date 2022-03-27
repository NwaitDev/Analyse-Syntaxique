#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <string.h>
#include "turtle-ast.h"

#define INITIAL_CAP 10


//Structure representing a dynamic tab of ast_nodes pointers
struct dynamic_tab{
    long capacity;
    long size;
    struct ast_node** data;
};

//instanciates a new dynamic tab
struct dynamic_tab *init_tab();

//destroys a dynamic tab
void destroy_tab(struct dynamic_tab* tab);

//adds an ast_node at the end of a dynamic tab
void append_node(struct dynamic_tab* tab, struct ast_node* node);

//returns the node that contains the name attribute passed as argument
struct ast_node *get_node(struct dynamic_tab *tab, const char* name);

#endif /* ARRAY_H */
