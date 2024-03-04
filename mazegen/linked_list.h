#ifndef LINKED_LIST
#define LINKED_LIST
#include "bi_node.h"

typedef struct Linked_list{    
    Node *first;    
    Node *last;
	unsigned int list_size;
}Linked_list;

void add(Linked_list*,void**);
void push(Linked_list*,void**);
void ordered_push(Linked_list* , void** ,int (*)(void*,void*));

void* get_in(Linked_list*,int);
Node* get_node_in(Linked_list*,int);
void* pop_last(Linked_list*);
void* pop_first(Linked_list*);

void delete_node(Linked_list*,Node*);
void delete_position(Linked_list*,int);
void destroy_list(Linked_list**);
void clear_list(Linked_list*);

Linked_list* new_linked_list();

#endif

