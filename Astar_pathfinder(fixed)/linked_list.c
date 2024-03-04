#include "linked_list.h"
linked_list * new_list(){
    linked_list * llist=malloc(sizeof(linked_list));
    llist->first_node=NULL;
    llist->size_list=0;
    return llist;
}

void insert_in(node **_node,void **object){
    node *new_node=malloc(sizeof(node));
    new_node->object=object;
    new_node->next_node=*_node;
    *_node=new_node;    
}

void insert(linked_list *llist,void **object){
    insert_in(&llist->first_node,object);
    llist->size_list++;
}

void order_insert(linked_list *llist,void **object,int (*compare)(const void *o1,const void *o2)){
    node *current_node=llist->first_node,*previous_node=NULL;
    while(current_node && compare(*current_node->object,*object)){
        previous_node=current_node;
        current_node=current_node->next_node;
    }
    insert_in(!previous_node?&llist->first_node:&previous_node->next_node,object);
    llist->size_list++;
}

int contains(linked_list *llist,void**target){
    node *_node = llist->first_node;
    while(_node){
        if(_node->object==*target)
            return 0;
        _node=_node->next_node;
    }
    return 1;
}

void ** remove_first_node(linked_list *llist){
    void **obj=llist->first_node->object;
    node *next=llist->first_node->next_node;

    free(llist->first_node);
    llist->first_node=next;
    llist->size_list--;
    return obj;
}

void destroy_list(linked_list **rllist){
    while ((*rllist)->first_node){
        node *next=(*rllist)->first_node->next_node;
        free(*(*rllist)->first_node->object);
        *(*rllist)->first_node->object=NULL;
        free((*rllist)->first_node);
        (*rllist)->first_node=next;
    }
    free((*rllist)->first_node);
    free(*rllist);
    *rllist=NULL;    
}
void clear_list(linked_list **rllist){
    while ((*rllist)->first_node){
        node *next=(*rllist)->first_node->next_node;
        free((*rllist)->first_node);
        (*rllist)->first_node=next;
    }    
}

void print_list(linked_list *list){
    node *itr_node=list->first_node;
    while (itr_node){
        printf("%d\n",**(int**)itr_node->object);
        itr_node=itr_node->next_node;
    }
}