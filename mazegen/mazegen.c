#include <stdlib.h>
#include "mazegen.h"

Stack m_stack;
void generate_maze(void*** grid,void*data0,void*data1,int H_LEN,int W_LEN){
    
    int pos[][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    Stack m_stack={ .index = 0};
    m_stack.elements=malloc((W_LEN*H_LEN)*sizeof(int**));
    for (int i = 0; i < H_LEN*W_LEN; i++)
        m_stack.elements[i]=malloc(2*sizeof(int*));

    int pivo[] = {5, 5};
    push(pivo,&m_stack);
    grid[pivo[0]][pivo[1]]=data1;
    while (m_stack.index>0){
        int k = 0;
        int neib[4][2];
        for (int i = 0; i < 4; i++){
            int r=pos[i][0]*2+pivo[0],c=pos[i][1]*2+pivo[1];
            if(r >= 0 && r < H_LEN && c >= 0 && c < W_LEN && grid[r][c]==data0){
                neib[k][0]=pos[i][0];
                neib[k++][1]=pos[i][1];
            }
        }
        if(k==0){
            int* p=pop(&m_stack);
            pivo[0]=p[0];
            pivo[1]=p[1];
        }
        else{
            int j=rand()%k;
            grid[pivo[0]+=neib[j][0]][pivo[1]+=neib[j][1]]=data1;
            grid[pivo[0]+=neib[j][0]][pivo[1]+=neib[j][1]]=data1;
            push(pivo,&m_stack);
        }
    }
    
    for (int i = 0; i < H_LEN*W_LEN; i++)
        free(m_stack.elements[i]);
    free(m_stack.elements);
}

int *pop(Stack *stack){
    return stack->index >= 0 ? stack->elements[--stack->index]:0;
}

void push(int *p, Stack *stack){
    stack->elements[stack->index][0] = p[0];
    stack->elements[stack->index++][1] = p[1];
}