#include <stdio.h>
#include <time.h>
#include "linked_list.h"

enum states{OPEN=1,CLOSED,BLOCKED,PATH,START,END};
const char *colors[]={"\033[47m","\033[42m","\033[43m","\033[47m","\033[45m","\033[46m","\033[41m"};
typedef struct {
    float mov_cost,dist_cost,total_cost;
    unsigned char state,pos_row,pos_col;
}cell;

cell ***grid_cell,start,end;
int row_len,col_len,ri,ci;

void init();
void print_grid();
node* search();

int main(int argc,char*argv[]){
    if(argc<3 || sscanf(argv[1],"%d",&row_len)==0 || sscanf(argv[2],"%d",&col_len) == 0)
        col_len=row_len=30;

    init();

    node *path=search();

    if(path)
        for(;path;path=path->next)
            grid_cell[((cell*)path->data)->pos_row][((cell*)path->data)->pos_col]->state=PATH;
    

    print_grid();
    return 0;
}

node* search(){
    
}

void init(){    
    grid_cell=malloc(sizeof(void*)*row_len);
    for(int i=0,j;i<row_len;i++){
        grid_cell[i]=malloc(sizeof(void*)*col_len);
        for (j = 0; j < col_len; j++)
            grid_cell[i][j]=NULL;
    }

    start.state=START;
    end.state=END;
    grid_cell[start.pos_row=3][start.pos_col=3] =&start;
    grid_cell[end.pos_row=row_len- 3][end.pos_col=col_len-3] =&end;
}

void print_grid(){    
    for(ri=0;ri<row_len;ri++){
        for ( ci = 0; ci < col_len; ci++){
            cell *cell_it=grid_cell[ri][ci];
            printf("%s  ",colors[cell_it?cell_it->state:0]);
        }
        puts("\033[0m");
    }
    
}