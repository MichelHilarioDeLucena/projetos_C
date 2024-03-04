typedef struct Stack{
    int **elements;
    int index;
} Stack;
int *pop(Stack *);
void push(int *, Stack *);
void generate_maze(void*** grid,void*data0,void*data1,int H_LEN,int W_LEN);
