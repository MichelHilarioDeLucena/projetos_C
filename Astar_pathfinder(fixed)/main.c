#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "linked_list.h"
#include "cell_path.h"

#define ROW_LEN 40
#define COL_LEN 40

void print_grid();

cell_path *pathfinder();

int row, col, neib[][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

cell_path *grid_cell[ROW_LEN][COL_LEN], *start, *end, *blocked_cell, *found_cell;
linked_list *open_cells, *closed_cells;

int main(){

    struct termios new_t, old_t;
    tcgetattr(STDIN_FILENO, &old_t);
    new_t = old_t;
    new_t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_t);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

    for (row = 0; row < ROW_LEN; row++)
        for (col = 0; col < COL_LEN; col++)
            grid_cell[row][col] = NULL;

    start = &(cell_path){.state = START, .x = 0, .y = 0, .g = 0.f},
    end = &(cell_path){.state = END, .x = 22, .y = 38},
    blocked_cell = &(cell_path){.state = BLOCKED};

    for (int k = 0; k < 20; k++)
        grid_cell[9][k + 9] = blocked_cell,
        grid_cell[20][k + 7] = blocked_cell,
        grid_cell[k + 6][17] = blocked_cell;

    grid_cell[start->y][start->x] = start;
    grid_cell[end->y][end->x] = end;
    open_cells = new_list();
    while (1){
        grid_cell[start->y][start->x] = NULL;
        switch (getchar()){
        case 'w':(--start->y) % ROW_LEN;break;
        case 's':(++start->y) % ROW_LEN;break;
        case 'a':(--start->x) % COL_LEN;break;
        case 'd':(++start->x) % COL_LEN;break;
        case 'q':
            tcsetattr(STDERR_FILENO, TCSANOW, &old_t);
            exit(0);
        }
        while (getchar() != EOF);

        grid_cell[start->y][start->x] = start;
        found_cell = pathfinder(&start, &end);
        start->state = START;

        while (found_cell && found_cell != start){
            found_cell->state = PATH;
            found_cell = found_cell->next;
        }
        clear_list(&open_cells);
        print_grid();
        usleep(80000);
        system("clear");
    }
    destroy_list(&open_cells);
    tcsetattr(STDERR_FILENO, TCSANOW, &old_t);
}

cell_path *pathfinder(cell_path **start, cell_path **end){
    cell_path *current_cell, **neib_cell;

    insert(open_cells, (void **)start);
    int index;
    while (open_cells->size_list > 0){
        current_cell = *remove_first_node(open_cells);
        current_cell->state = CLOSED;

        for (index = 0; index < 8; index++){
            int neib_row = current_cell->y + neib[index][0],
                neib_col = current_cell->x + neib[index][1],
                in_range = neib_row >= 0 && neib_row < ROW_LEN && neib_col >= 0 && neib_col < COL_LEN;
            if (!in_range )
                continue;

            float new_step_cost = current_cell->g + (neib[index][0] * neib[index][1] ? 1.4142f : 1.f)
                ,exist=0;
            neib_cell = &grid_cell[neib_row][neib_col];

            if (*neib_cell == *end)
                return current_cell;

            if (!*neib_cell){
                (*neib_cell) = malloc(sizeof(cell_path));
                (*neib_cell)->next = current_cell;
                (*neib_cell)->state = OPEN;
                (*neib_cell)->y = neib_row;
                (*neib_cell)->x = neib_col;
                (*neib_cell)->g = new_step_cost;
                (*neib_cell)->h = heur(*neib_cell, *end);
                (*neib_cell)->f = new_step_cost + (*neib_cell)->h;
                order_insert(open_cells, (void **)neib_cell, compare_cell);
            }else if((*neib_cell)->state==OPEN && new_step_cost<(*neib_cell)->g){
                (*neib_cell)->next=current_cell;
                (*neib_cell)->g=new_step_cost;
                (*neib_cell)->f = new_step_cost + (*neib_cell)->h;
                order_insert(open_cells, (void **)neib_cell, compare_cell);
            }
        }
    }
    return NULL;
}

void print_grid(){
    for (row = 0; row < ROW_LEN; row++, puts("\033[0m"))
        for (col = 0; col < COL_LEN; col++){
            if (!grid_cell[row][col]){
                printf("\033[48;5;%dm  ", WHITE);
                continue;
            }
            printf("\033[48;5;%dm  ", grid_cell[row][col]->state);
            if (grid_cell[row][col] != start && grid_cell[row][col] != end && grid_cell[row][col] != blocked_cell){
                free(grid_cell[row][col]);
                grid_cell[row][col] = NULL;
            }
        }
}
