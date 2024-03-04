#include <stdio.h>
#include <stdlib.h>

#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct node{
    void **object;
    struct node *next_node;
}node;

typedef struct linked_list{
    unsigned int size_list;
    node *first_node;    
}linked_list;

linked_list * new_list();
void insert(linked_list *llist,void **object);
void insert_in(node **_node,void **object);
int contains(linked_list *llist,void**obj);
void order_insert(linked_list *llist,void **object,int (*compare)(const void *o1,const void *o2));
void ** remove_first_node(linked_list *llist);
void print_list(linked_list *list);
void destroy_list(linked_list **llist);
void clear_list(linked_list **llist);
#endif