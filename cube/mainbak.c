#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#define X_ 0
#define Y_ 1
#define Z_ 2
#define W_ 3
#define MIN_P(a, b, c) a[c] < b[c] ? a : b
#define MAX_P(a, b, c) a[c] > b[c] ? a : b

#define PI 3.1415926f

// 232 preto - 255 branco
#define SHADE0 "\033[48;5;233m  "
#define SHADE1 "\033[48;5;245m  "
#define SHADE2 "\033[48;5;255m  "

#define BLACK "\033[40m  "
#define RED "\033[41m  "
#define GREEN "\033[42m  "
#define YELLOW "\033[43m  "
#define BLUE "\033[44m  "
#define MAGENTA "\033[45m  "
#define CYAN "\033[46m  "
#define WHITE "\033[47m  "
 
const char *colors[] = {BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW,WHITE,"\033[48;5;226m  "};

bool onLine(float l1[2][2], float p[2]); 
int direction(float a[2], float b[2],float c[2]); 
bool isIntersect(float l1[2][2], float l2[2][2]);
bool checkInside(float **poly, int n, float p[2]);
// DDA alg
void draw_line(size_t m_col, short *scr, float p0[2], float p1[2],int color);
void fill_rect(short *scr, float **rect,int color);

int main()
{
    int n_row = 100, n_col = 100, total_len = n_row * n_col;
    short screen[total_len];
    for (short *pointer = screen; pointer < screen + total_len; pointer++)
        *pointer = 0;
    float
        cube_verts[][3] = {
            {-.5f, -.5f, .5f}, {.5f, -.5f, .5f}, {.5f, .5f, .5f}, {-.5f, .5f, .5f},
            {-.5f, -.5f, -.5f}, {.5f, -.5f, -.5f}, {.5f, .5f, -.5f}, {-.5f, .5f, -.5f}
        },        
        cube_proj[8][3]={0},
        cube_transf[8][3]={0},        
        *faces[6][4]={
            {cube_proj[0],cube_proj[1],cube_proj[2],cube_proj[3]},
            {cube_proj[1],cube_proj[5],cube_proj[6],cube_proj[2]},
            {cube_proj[4],cube_proj[5],cube_proj[6],cube_proj[7]},
            {cube_proj[4],cube_proj[7],cube_proj[3],cube_proj[0]},
            {cube_proj[0],cube_proj[1],cube_proj[5],cube_proj[4]},
            {cube_proj[3],cube_proj[2],cube_proj[6],cube_proj[7]}
        },s_faces[6],
        near = .5f,
        far = 1000.f,
        fov = 1.f / tanf(.25f * PI),
        aspec_ratio = (float)n_row / (float)n_col,
        mx_proj = (aspec_ratio * fov),
        my_proj = fov,
        mz_proj = far / (far - near),
        mw_proj = -(far * near) / (far - near),
        theta=PI/32;

       //((size_t)faces[i][j]-(size_t)cube_proj)/12
    while(1){
        for (short *pointer  = screen; pointer < screen+total_len; pointer++)*pointer=0;
        theta+=.11f;
        float sin_=sinf(theta),cos_=cosf(theta);
        for (size_t i = 0; i < 8; i++){
            cube_transf[i][X_]=cube_verts[i][X_]*cos_+cube_verts[i][Z_]*sin_;
            cube_transf[i][Y_]=sin_*sin_*cube_verts[i][X_]+cos_*cube_verts[i][Y_]-sin_*cos_*cube_verts[i][Z_];
            cube_transf[i][Z_]=-cos_*sin_*cube_verts[i][X_]+sin_*cube_verts[i][Y_]+cube_verts[i][Z_]*cos_*cos_;

            float
                px=cube_transf[i][X_]*mx_proj,
                py=cube_transf[i][Y_]*my_proj,
                pz=cube_transf[i][Z_]*mz_proj,
                pw=mw_proj+cube_transf[i][Z_]+3;

            cube_proj[i][X_]=(px*n_col/pw)+55;
            cube_proj[i][Y_]=(py*n_row/pw)+55;
            cube_proj[i][Z_]=pz;
        }

        
/*
        //GAMBIARRA INICIO
        for (int i = 0; i < 6; i++)    
            s_faces[i]=fmax(fmaxf(fmaxf(faces[i][0][Z_],faces[i][1][Z_]),faces[i][2][Z_]),faces[i][3][Z_]);
        
        float tmp,*ptemp[4];
        int j;
        for (int i = 1; i < 6; i++)
        {
            tmp = s_faces[i];
            for (size_t k = 0; k < 4; k++)
                ptemp[k]=faces[i][k];            
            
            for (j = i - 1; j >= 0 && tmp < s_faces[j]; j--)
            {
                s_faces[j + 1] = s_faces[j];

                for (size_t k = 0; k < 4; k++)
                    faces[j+1][k]=faces[j][k];
            }
            s_faces[j + 1] = tmp;
            for (size_t k = 0; k < 4; k++)
                faces[j + 1][k] = ptemp[k];
        }
        //GAMBIARRA FIM

*/

        const int q=1,s=3,r=0;


        for (int i = 5; i>=0; i--){
            
            int q_i=((size_t)faces[i][q]-(size_t)cube_proj)/12;
            int r_i=((size_t)faces[i][r]-(size_t)cube_proj)/12;
            int s_i=((size_t)faces[i][s]-(size_t)cube_proj)/12;
            float
                v2[]={
                    cube_transf[q_i][X_]-cube_transf[r_i][X_],
                    cube_transf[q_i][Y_]-cube_transf[r_i][Y_],
                    cube_transf[q_i][Z_]-cube_transf[r_i][Z_]
                },            
                v1[]={
                    cube_transf[s_i][X_]-cube_transf[r_i][X_],
                    cube_transf[s_i][Y_]-cube_transf[r_i][Y_],
                    cube_transf[s_i][Z_]-cube_transf[r_i][Z_]
                };
            float normal_vec[]={
                v1[Y_]*v2[Z_]-v2[Y_]*v1[Z_],
                -(v1[X_]*v2[Z_]-v2[X_]*v1[Z_]),
                v1[X_]*v2[Y_]-v2[X_]*v1[Y_]
            };

            float
                px=normal_vec[X_]*mx_proj,
                py=normal_vec[Y_]*my_proj,
                pz=normal_vec[Z_]*mz_proj,
                pw=mw_proj+normal_vec[Z_]+3;

            normal_vec[X_]=(px*n_col/pw)+55;
            normal_vec[Y_]=(py*n_row/pw)+55;
            normal_vec[Z_]=pz;
            // printf("{%f %f %f}\n",normal_vec[X_],normal_vec[Y_],normal_vec[Z_]);
/*
            float
                px=cube_transf[i][X_]*mx_proj,
                py=cube_transf[i][Y_]*my_proj,
                pz=cube_transf[i][Z_]*mz_proj,
                pw=mw_proj+cube_transf[i][Z_]+5;

            normal_vec[X_]+=(px*n_col/pw)+55;
            normal_vec[Y_]+=(py*n_row/pw)+55;
            normal_vec[Z_]+=pz;
*/
            
            // float module=sqrtf(normal_vec[X_]*normal_vec[X_]+normal_vec[Y_]*normal_vec[Y_]*normal_vec[Z_]*normal_vec[Z_]);
            // normal_vec[X_]/=module;
            // normal_vec[Y_]/=module;
            // normal_vec[Z_]/=module;
            if((
                
                normal_vec[Z_]-1)>=0.00001f)continue;
             
            fill_rect(screen,faces[i],i+1);
            // draw_line(n_col,screen,faces[i][0],faces[i][1],7);
            // draw_line(n_col,screen,faces[i][0],faces[i][3],7);
            // draw_line(n_col,screen,faces[i][2],faces[i][3],7);
            // draw_line(n_col,screen,faces[i][2],faces[i][1],7);
            draw_line(n_col,screen,normal_vec,faces[i][r],i+1);    
        }
    
        for (size_t i = 0; i < total_len; i++)
            printf("%s",i%n_col==0?"\n":colors[screen[i]]);
        usleep(150000);
        system("clear");
    }
}

void draw_line(size_t m_col, short *scr, float p0[2], float p1[2],int c)
{
    float dx = p1[X_] - p0[X_], dy = p1[Y_] - p0[Y_];
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float ix = dx / steps;
    float iy = dy / steps;
    float x = p0[X_], y = p0[Y_];
    for (size_t i = 0; i <= steps; i++)
        scr[(int)roundf(x) * m_col + (int)roundf(y)] = c, x += ix, y += iy;
}
void draw_poly2D(size_t w, short *pnl, size_t n_vert, float poly[n_vert][2], size_t start)
{
    for (size_t point = start + 1; point < n_vert; point++)
        draw_line(w, pnl, poly[point - 1], poly[point],2);
    draw_line(w, pnl, poly[n_vert - 1], poly[start],2);
}
bool onLine(float l1[2][2], float p[2])
{
    if (p[X_] <= fmax(l1[0][X_], l1[1][X_])
        && p[X_] >= fmin(l1[0][X_], l1[1][X_])
        && (p[Y_] <= fmax(l1[0][Y_], l1[1][Y_])
            && p[Y_] >= fmin(l1[0][Y_], l1[1][Y_])))
        return true; 
    return false;
}
int direction(float a[2], float b[2],float c[2])
{
    float val = (b[Y_] - a[Y_]) * (c[X_] - b[X_])
              - (b[X_] - a[X_]) * (c[Y_] - b[Y_]); 
    return val==0?0:val<0?2:1;
}

bool isIntersect(float l1[2][2], float l2[2][2])
{ 
    int dir1 = direction(l1[0], l1[1], l2[0]);
    int dir2 = direction(l1[0], l1[1], l2[1]);
    int dir3 = direction(l2[0], l2[1], l1[0]);
    int dir4 = direction(l2[0], l2[1], l1[1]);
 
    if (dir1 != dir2 && dir3 != dir4)
        return true;
    if (dir1 == 0 && onLine(l1, l2[0]))
        return true;
    if (dir2 == 0 && onLine(l1, l2[1]))
        return true;
    if (dir3 == 0 && onLine(l2, l1[0]))
        return true;
    if (dir4 == 0 && onLine(l2, l1[1]))
        return true; 
    return false;
}
bool checkInside(float **poly, int n, float p[2])
{
    if (n < 3)
        return false;
    float exline[][2] = { {p[0],p[1]}, { 9999, p[0] } };
    int count = 0;
    int i = 0;
    do {
        float side[2][2] = { {poly[i][0],poly[i][1]}, poly[(i + 1) % n][0],poly[(i + 1) % n][1] };
        if (isIntersect(side, exline)) { 
            if (direction(side[0], p, side[1]) == 0)
                return onLine(side, p);
            count++;
        }
        i = (i + 1) % n;
    } while (i != 0);
    return count & 1;
}
void fill_rect(short *scr, float **rect,int c)
{
    int
        max_x=fmaxf(fmaxf(fmaxf(rect[0][X_],rect[1][X_]),rect[2][X_]),rect[3][X_]),
        min_x=fminf(fminf(fminf(rect[0][X_],rect[1][X_]),rect[2][X_]),rect[3][X_]),

        max_y=fmaxf(fmaxf(fmaxf(rect[0][Y_],rect[1][Y_]),rect[2][Y_]),rect[3][Y_]),
        min_y=fminf(fminf(fminf(rect[0][Y_],rect[1][Y_]),rect[2][Y_]),rect[3][Y_]);

    for (int i = min_y; i <= max_y; i++)
        for (int j = min_x; j <= max_x; j++)
            if(checkInside(rect,4,(float[]){j,i}))
                scr[(int)roundf(j) * 100 + (int)roundf(i)] = c;
        
}
