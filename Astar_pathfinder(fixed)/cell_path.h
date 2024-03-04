#ifndef CELL_PATH
#define CELL_PATH
#define SQRT2 1.4142f
#include <math.h>

typedef struct cell_path{
    short x,y;
    float g,h,f;
    unsigned char state;
    struct cell_path* next;
}cell_path;

enum colors{RED=196,GREEN=112,BLUE=33,YELLOW=226,MAGENTA=201,BLACK=232,WHITE=255,CYAN=51,ORANGE=172};
enum states{OPEN=GREEN,CLOSED=RED,START=CYAN,END=BLUE,PATH=YELLOW,BLOCKED=BLACK};

int compare_cell(const void *obj1,const void *obj2);
float heur(cell_path *a,cell_path *b);
#endif