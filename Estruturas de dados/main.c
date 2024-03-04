#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linked_list.h"

int _compare(void*a,void*b){    
    return *(int*)a<*(int*)b;
}

int main(){
    Linked_list* my_list=new_linked_list();
    srand(time(0));
    for(int i=0;i<10;i++){
        int* j=malloc(sizeof(int));    
        *j=rand()%99;
        printf("%d ",*j);
        // add(my_list,j);
        ordered_push(my_list,j,_compare);
    }
    int*o=(int*)pop_first(my_list);
    printf("valor popado%d\n",*o);
    // puts("");
    // int j=4;
    // return 0;
    // push(my_list,p);
    
    //delete_node(my_list,get_node_in(my_list,1));
    // delete_position(my_list,6);
    //printf("fnp:%c",my_list->first->next->previous?'v':'n');
    //destroy_list(&my_list);
    // int* v=(int*)pop_last(my_list);
    // printf("\nteste pop:%d\n",v?*v:0);
    
    // printf("\n\033[44m\ndebug last node:\nlast->prev:%p\nlast->next:%p\nlast->value:%d\n\033[0m",
    //  my_list->last->previous,
    //  my_list->last->next,
    //  *(int*)my_list->last->value);

    // printf("\ntamanho:%d |primeiro:%d |ultimo:%d\n",
	// my_list?my_list->list_size:0,
    // my_list?*(int*)my_list-> first->next->value:0,
    // my_list?*(int*)my_list->last->value:0);
    // int j=4;
    // printf("\n\nindice:%d\nvalor:%d\nanterior:%d\n",j,
    // my_list?*(int*)get_in(my_list,j):0,
    // my_list?(get_node_in(my_list,j)->previous->value)?
    // *(int*)get_node_in(my_list,j)->previous->value:0:0);
    // puts(my_list?"não nulo":"nulo");
    if(!my_list)return 0;
	Node* node=my_list->last;
    do{
        printf("\033[43m %d\033[0m",*(int*)node->value);		
	}while((node=node->previous)&&node->value);
    puts("");
}

