#include "array.h"

struct dynamic_tab *init_tab(){
    struct dynamic_tab *tab = malloc(sizeof(struct dynamic_tab)); //allocates memory for the structure
    tab->size = 0;
    tab->capacity = INITIAL_CAP;
    tab->data = malloc(INITIAL_CAP*sizeof(struct ast_node*)); //allocates the memory for the data
    return tab;
}

void destroy_tab(struct dynamic_tab* tab){
    free(tab->data); //frees up space taken by the data
    tab->size = 0;
    tab->capacity = 0;
    free(tab); //frees up space taken by the structure
}

void append_node(struct dynamic_tab *tab, struct ast_node* node){

    if(tab->size == tab->capacity){ //when the dynamic tab is full
        struct ast_node** new_data = malloc(tab->capacity*2*sizeof(struct ast_node*)); //allocates bigger space for the data
        memcpy(new_data,tab->data,tab->capacity*sizeof(struct ast_node)); //copying the data into the new space
        tab->capacity*=2;
        free(tab->data); //freeing up old space taken by the data
        tab->data = new_data; //associating new space to the current strcture
    }
    tab->data[tab->size++] = node; //adding the node to the structure
}

struct ast_node *get_node(struct dynamic_tab *tab, const char* name){
    for(size_t i = 0; i<tab->size; ++i){ //going through the dynamic tab until the data has the name we're looking for
        if(strcmp(tab->data[i]->u.name,name)==0){
            return tab->data[i];
        }
    }
    return NULL; //in case we do not find the node, we return NULL
}