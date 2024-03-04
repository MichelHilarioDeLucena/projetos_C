#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

Linked_list* new_linked_list(){
    Linked_list* linked_list=malloc(sizeof(Linked_list));
    linked_list->first=malloc(sizeof(Node));    
    linked_list->last=linked_list->first;    
    linked_list->list_size=0;
    return linked_list;
}
void add(Linked_list* linked_list, void** _value){
    Node* new_node=malloc(sizeof(Node)),*prev_node=linked_list->last;    
    new_node->previous=prev_node;    
    new_node->value=_value;
    new_node->next=NULL;
    linked_list->last->next=new_node;
    linked_list->last=new_node;
    linked_list->list_size++;
}

void push(Linked_list* linked_list, void** _value){
    Node* new_node=malloc(sizeof(Node));
    if(!linked_list->first->next)
        return add(linked_list,_value);
    new_node->value=_value;
    new_node->next=linked_list->first->next;
    linked_list->first->next->previous=new_node;
    linked_list->first->next=new_node;
    linked_list->list_size++;
}

void ordered_push(Linked_list* linked_list, void** _value,int (*compare)(void*,void*)){
        
    Node* limit_node=linked_list->last;
    while (limit_node->value&&compare(*limit_node->value,*_value))
        limit_node=limit_node->previous;    
    if(!linked_list->last->value||limit_node==linked_list->last)
        return add(linked_list,_value);        
    
    Node* new_node=malloc(sizeof(Node));
    new_node->value=_value;
    new_node->previous=limit_node;
    
    new_node->next=limit_node->next;
    limit_node->next->previous=new_node;
    limit_node->next=new_node;

    linked_list->list_size++;
}

void delete_node(Linked_list* linked_list,Node* _node){
    Node*this_node=_node;
    if(_node->next){
        _node->next->previous=_node->previous;
        _node->previous->next=_node->next;
    }else{
        linked_list->last=_node->previous;
        _node->previous->next=NULL;
    }
    // free(*this_node->value);
    // *this_node->value=NULL;
    free(this_node);    
    linked_list->list_size--;
}

void delete_position(Linked_list* linked_list,int index){        
    index%=linked_list->list_size; 
    Node * currt=linked_list->first->next; 
    for (int i = 0; i < index; i++)
        currt=currt->next;    
    if(currt->next){
        currt->next->previous=currt->previous;
        currt->previous->next=currt->next;    
    } else {
        linked_list->last=currt->previous;
        currt->previous->next=NULL;
    }
    free(*currt->value);
    *currt->value=NULL;
    // free(currt->value);
    // currt->value=NULL;
    free(currt);
    linked_list->list_size--;    
}
void* get_in(Linked_list* linked_list,int index){           
    return *get_node_in(linked_list,index)->value;
}
Node* get_node_in(Linked_list* linked_list,int index){       
    index%=linked_list->list_size; 
    Node * currt=linked_list->first->next;    
    for (int i = 0; i < index; i++)
        currt=currt->next;
    return currt;
}

void* pop_last(Linked_list* list){
    void* value=*list->last->value;
    
    delete_node(list,list->last);
    return value;    
}
void* pop_first(Linked_list* list){
    void* value=*list->first->next->value;    
    delete_node(list,list->first->next);
    return value;    
}

void destroy_list(Linked_list** linked_list){    
    clear_list(*linked_list);
    free(*linked_list);
    (*linked_list)=NULL;
}

void clear_list(Linked_list* linked_list){
   while(linked_list->list_size>0)
        delete_position(linked_list,0);
}
