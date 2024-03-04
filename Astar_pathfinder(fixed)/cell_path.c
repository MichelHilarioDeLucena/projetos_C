#include <stdlib.h>
#include "cell_path.h"

const float DIAGONAL_VALUE=SQRT2;
const float DVALUE=SQRT2 - 2;

int compare_cell(const void *obj1,const void *obj2){
    return   ((cell_path*)obj1)->f < ((cell_path*)obj2)->f;
}
float heur(cell_path *a,cell_path *b){

    short dx = abs(a->x - b->x),
        dy = abs(a->y - b->y);
    return (dx + dy) + DVALUE * (dx>dy?dy:dx);

    // int dx =  a->x - b->x, dy = a->y - b->y;
    // return sqrtf(dx*dx + dy*dy);

    // int dx =  abs(a->x - b->x), dy = abs(a->y - b->y);    
    // return dx<dy ? (1.4f*dy+dx) : (1.4f*dx+dy);
}