#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.1415926f
// 232 preto - 255 branco
#define SHADE0 "\033[48;5;233m  "
#define SHADE1 "\033[48;5;245m  "
#define SHADE2 "\033[48;5;255m  "

#define BLACK "\033[40m  "
#define RED "\033[41m  "
#define GREEN "\033[42m  "
#define YELLOW "\033[33;43m  "
#define BLUE "\033[44m  "
#define MAGENTA "\033[45m  "
#define CYAN "\033[46m  "
#define WHITE "\033[47m  "

typedef struct vec3d
{
    float x, y, z;
} vec3d;

typedef struct triangle
{
    vec3d points[3];
} triangle;

typedef struct mesh
{
    triangle *m_triang;
} mesh;

typedef struct mat4x4
{
    float m[4][4];
} mat4x4;

enum{BLACK_ID,RED_ID,GREEN_ID,BLUE_ID,CYAN_ID,MAGENTA_ID,YELLOW_ID,WHITE_ID};
const char *shaders[] = {BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};

void vecXmat(vec3d *input, vec3d *output, mat4x4 *m);
// DDA alg
void draw_line(size_t m_col, short *scr, vec3d *p0, vec3d *p1, int value);

int main()
{

    int n_row = 80, n_col = 100, total_len = n_row * n_col;
    short screen[total_len];
    mesh cube_mesh = {.m_triang = (triangle[]){
                          {.points = {{0, 0, 0}, {0, 1, 0}, {1, 1, 0}}},
                          {.points = {{0, 0, 0}, {1, 1, 0}, {1, 0, 0}}},
                          {.points = {{1, 0, 0}, {1, 1, 0}, {1, 1, 1}}},
                          {.points = {{1, 0, 0}, {1, 1, 1}, {1, 0, 1}}},
                          {.points = {{1, 0, 1}, {1, 1, 1}, {0, 1, 1}}},
                          {.points = {{1, 0, 1}, {0, 1, 1}, {0, 0, 1}}},
                          {.points = {{0, 0, 1}, {0, 1, 1}, {0, 1, 0}}},
                          {.points = {{0, 0, 1}, {0, 1, 0}, {0, 0, 0}}},
                          {.points = {{0, 1, 0}, {0, 1, 1}, {1, 1, 1}}},
                          {.points = {{0, 1, 0}, {1, 1, 1}, {1, 1, 0}}},
                          {.points = {{1, 0, 1}, {0, 0, 1}, {0, 0, 0}}},
                          {.points = {{1, 0, 1}, {0, 0, 0}, {1, 0, 0}}}}};

    float
        near = .5f, 
        far = 1000.f,
        fov = 1.f / tanf(.25f * PI),
        apesc_r = (float)n_row / n_col,
        px = apesc_r * fov,
        py = .5f * PI,
        pz = far / (far - near), 
        pw = (-far * near) / (far - near);

    mat4x4 mat_proj = {{{px, 0, 0, 0},
                        {0, py, 0, 0},
                        {0, 0, pz, 1},
                        {0, 0, 0, pw}}};

    float theta=PI/32.f, *cos_x=malloc(sizeof(float)) , *sin_x=malloc(sizeof(float));

    
    while (1)
    {
        theta+=.1f;
        *cos_x=cosf(theta),*sin_x=sinf(theta);
        for (short *pointer = screen; pointer < screen + total_len; pointer++)
            *pointer = 0;

            mat4x4 mat_rotx = {{{ 1 ,  0  ,  0  ,  0   },
                                { 0 , *cos_x, *sin_x ,0},
                                { 0 ,-*sin_x, *cos_x, 0},
                                { 0 ,  0  ,  0  ,  1   }}};
        for (int i = 0; i < 12; i++)
        {
            triangle tri_p, tri_r;        
            vecXmat(&cube_mesh.m_triang[i].points[0], &tri_r.points[0], &mat_rotx);
            vecXmat(&cube_mesh.m_triang[i].points[1], &tri_r.points[1], &mat_rotx);
            vecXmat(&cube_mesh.m_triang[i].points[2], &tri_r.points[2], &mat_rotx);

            triangle tri_t ={{tri_r.points->x,tri_r.points->y,tri_r.points->z}};
            tri_t.points[0].z += 3.f;
            tri_t.points[1].z += 3.f;
            tri_t.points[2].z += 3.f;

            vecXmat(&tri_t.points[0], &tri_p.points[0], &mat_proj);
            vecXmat(&tri_t.points[1], &tri_p.points[1], &mat_proj);
            vecXmat(&tri_t.points[2], &tri_p.points[1], &mat_proj);
            // vec3d *debug_tt=cube_mesh.m_triang[i].points;            

            tri_p.points[0].x+=1.f,tri_p.points[0].y+=1.f;
            tri_p.points[1].x+=1.f,tri_p.points[1].y+=1.f;
            tri_p.points[2].x+=1.f,tri_p.points[2].y+=1.f;

            tri_p.points[0].x*=.5f*n_col,tri_p.points[0].y*=.5f*n_row;
            tri_p.points[1].x*=.5f*n_col,tri_p.points[1].y*=.5f*n_row;
            tri_p.points[2].x*=.5f*n_col,tri_p.points[2].y*=.5f*n_row;

            vec3d *debug_tt=tri_p.points;
            printf("{%.1f %.1f %.1f}  {%.1f %.1f %.1f}  {%.1f %.1f %.1f}\n",
                debug_tt[0].x,debug_tt[0].y,debug_tt[0].z,
                debug_tt[1].x,debug_tt[1].y,debug_tt[1].z,
                debug_tt[2].x,debug_tt[2].y,debug_tt[2].z
            );

            draw_line(n_col,screen,&tri_p.points[0],&tri_p.points[1],WHITE_ID);
            // draw_line(n_col,screen,&tri_p.points[1],&tri_p.points[2],WHITE_ID);
            // draw_line(n_col,screen,&tri_p.points[2],&tri_p.points[0],WHITE_ID);
        }
        
    
        for (size_t i = 0; i < total_len; i++)
            printf("%s", i % n_col ? shaders[screen[i]] : "\n");
        puts("\033[0m");

        usleep(350000);
        system("clear");
    }
}

void vecXmat(vec3d *input, vec3d *output, mat4x4 *mat)
{
    output->x = input->x * mat->m[0][0] + input->y * mat->m[1][0] + input->z * mat->m[2][0] + mat->m[3][0];
    output->y = input->x * mat->m[0][1] + input->y * mat->m[1][1] + input->z * mat->m[2][1] + mat->m[3][1];
    output->z = input->x * mat->m[0][2] + input->y * mat->m[1][2] + input->z * mat->m[2][2] + mat->m[3][2];
    float w   = input->x * mat->m[0][3] + input->y * mat->m[1][3] + input->z * mat->m[2][3] + mat->m[3][3];
    if (w != 0.0f)
        output->x /= w, output->y /= w, output->z /= w;
}

void draw_line(size_t m_col, short *scr, vec3d *p0, vec3d *p1, int value)
{
    
    float dx = p1->x - p0->x, dy = p1->y - p0->y;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float ix = dx / steps;
    float iy = dy / steps;
    float x = p0->x, y = p0->y;
    for (size_t i = 0; i <= steps; i++)
        scr[((int)roundf(x) * m_col + (int)roundf(y))] = value, x += ix, y += iy;
}
