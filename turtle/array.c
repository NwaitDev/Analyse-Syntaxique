#include "array.h"

struct dynamic_tab *init_tab(){
    struct dynamic_tab *tab = malloc(sizeof(struct dynamic_tab));
    tab->size = 0;
    tab->capacity = INITIAL_CAP;
    tab->data = malloc(INITIAL_CAP*sizeof(struct ast_node*));
    return tab;
}

void destroy_tab(struct dynamic_tab* tab){
    free(tab->data);
    tab->size = 0;
    tab->capacity = 0;
    free(tab);
}

void append_node(struct dynamic_tab *tab, struct ast_node* node){
    if(tab->size == tab->capacity){
        struct ast_node** new_data = malloc(tab->capacity*2*sizeof(struct ast_node*));
        memcpy(new_data,tab->data,tab->capacity*sizeof(struct ast_node));
        tab->capacity*=2;
        free(tab->data);
        tab->data = new_data;
    }
    tab->data[tab->size++] = node;
}

struct ast_node *get_node(struct dynamic_tab *tab, const char* name){
    for(size_t i = 0; i<tab->size; ++i){
        if(strcmp(tab->data[i]->u.name,name)==0){
            return tab->data[i];
        }
    }
    return NULL;
}