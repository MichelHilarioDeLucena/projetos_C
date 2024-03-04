#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define X_ 0
#define Y_ 1
#define Z_ 2
#define W_ 3
#define MIN_P(a, b, c) a[c] < b[c] ? a : b
#define MAX_P(a, b, c) a[c] > b[c] ? a : b
#define PI 3.1415926f

#define BLACK   "\033[0m  "
#define GRAY    "\033[48;5;235m  "
#define RED     "\033[48;5;196m  "
#define GREEN   "\033[48;5;46m  "
#define YELLOW  "\033[48;5;226m  "
#define BLUE    "\033[48;5;21m  "
#define MAGENTA "\033[48;5;199m  "
#define CYAN    "\033[48;5;45m  "
#define WHITE   "\033[48;5;255m  "

#define RSHADE0  "\033[48;5;16m  "
#define RSHADE1  "\033[48;5;233m  "
#define RSHADE2  "\033[48;5;234m  "
#define RSHADE3  "\033[48;5;52m  "
#define RSHADE4  "\033[48;5;88m  "
#define RSHADE5  "\033[48;5;124m  "
#define RSHADE6  "\033[48;5;160m  "
#define RSHADE7  "\033[48;5;196m  "
#define RSHADE8  "\033[48;5;1m  "
#define RSHADE9  "\033[48;5;210m  "


// 232 preto - 255 branco
#define SHADE0  "\033[48;5;232m  "
#define SHADE1  "\033[48;5;233m  "
#define SHADE2  "\033[48;5;234m  "
#define SHADE3  "\033[48;5;235m  "
#define SHADE4  "\033[48;5;236m  "
#define SHADE5  "\033[48;5;237m  "
#define SHADE6  "\033[48;5;238m  "
#define SHADE7  "\033[48;5;239m  "
#define SHADE8  "\033[48;5;240m  "
#define SHADE9  "\033[48;5;241m  "
#define SHADE10 "\033[48;5;242m  "
#define SHADE11 "\033[48;5;243m  "
#define SHADE12 "\033[48;5;244m  "
#define SHADE13 "\033[48;5;245m  "
#define SHADE14 "\033[48;5;246m  "
#define SHADE15 "\033[48;5;247m  "
#define SHADE16 "\033[48;5;248m  "
#define SHADE17 "\033[48;5;249m  "
#define SHADE18 "\033[48;5;250m  "
#define SHADE19 "\033[48;5;251m  "
#define SHADE20 "\033[48;5;252m  "
#define SHADE21 "\033[48;5;253m  "
#define SHADE22 "\033[48;5;254m  "

typedef struct Object3D{
    int n_verts;
    int n_faces;
    float **verts;
    float **p_verts;
    float **t_verts;
    float ***faces;
}Object3D;

typedef struct Display{
    float   
        far,near,fov,
        mx_proj,
        my_proj,
        mz_proj,
        mw_proj;
    int n_col,n_row;
    short *screen;
}Display;


int onLine(float l1[2][2], float p[2]);
int direction(float a[2], float b[2], float c[2]);
int isIntersect(float l1[2][2], float l2[2][2]);
int checkInside(float **poly, int n, float p[2]);
// DDA alg
void draw_line(int m_col, short *scr, float *p0, float *p1, int value);
void fill_rect(short *scr, float **rect, int shade);

void draw_object3d(Object3D* cube,Display *display,float sin_,float cos_,float *light,int clen);

