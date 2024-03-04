#include "linked_list.h"

linked_list * new_linked_list(){
    linked_list *list= malloc(sizeof(linked_list));
    list->size=0;
    list->head=NULL;
}

void insert_list(linked_list *list,void *data){
    node *new_node=malloc(sizeof(node));
    new_node->data=data;
    new_node->next=list->head;
    list->head=new_node;
}

void sorted_insert_list(linked_list* list,void*data,int(*comp)(const void *a,const void *b)){

    node *it_node=list->head,*ant_node=NULL;
    while(it_node && comp(data,it_node->data)>0){
        ant_node=it_node;
        it_node=it_node->next;        
    }

    node *new_node=malloc(sizeof(node));
    new_node->data=data;
    new_node->next=it_node;

    if(ant_node)ant_node->next=new_node;
    else list->head=new_node;
}