#ifndef LINKED_LIST
#define LINKED_LIST
#include <stdlib.h>

typedef struct node{
    void* data;
    struct node* next;
}node;

typedef struct linked_list{
    unsigned int size;
    node *head;
}linked_list;

linked_list * new_linked_list();

void insert_list(linked_list* list,void*data);
void sorted_insert_list(linked_list* list,void*data,int(*comp)(const void *a,const void *b));
#endif