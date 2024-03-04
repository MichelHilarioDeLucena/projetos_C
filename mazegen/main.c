
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include "mazegen.h"
int main(){
    
    srand(time(NULL));

    int* visited=malloc(sizeof(int)),*not_visited=malloc(sizeof(int));

    *visited=9123;
    *not_visited=23453;

    int H_LEN=29,W_LEN=39;
    int ***my_grid;

    my_grid=malloc(H_LEN*sizeof(void**));

    for (int i = 0; i < H_LEN; i++)
        my_grid[i]=malloc(W_LEN*sizeof(void*));

    generate_maze((void***)my_grid,visited, NULL,H_LEN,W_LEN);

    for (int i = 0; i < H_LEN; i++){
        for (int j = 0; j < W_LEN; j++)
            printf(my_grid[i][j]==NULL? "\033[44m  ":"\033[47m..");        
        printf("\033[40m\n");
    }

}