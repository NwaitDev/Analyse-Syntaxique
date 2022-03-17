#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <string.h>
#include "turtle-ast.h"

#define INITIAL_CAP 10



struct dynamic_tab{
    long capacity;
    long size;
    struct ast_node** data;
};

struct dynamic_tab *init_tab();

void destroy_tab(struct dynamic_tab* tab);

void append_node(struct dynamic_tab* tab, struct ast_node* node);

struct ast_node *get_node(struct dynamic_tab *tab, const char* name);

#endif /* ARRAY_H */
