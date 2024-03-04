#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include "my_graphics.h"

#define SHADE23 "\033[48;5;255m  "
#define CINDEX(r,c,w) r*w+c
// const char *colors[] = {SHADE0,SHADE1,SHADE2,SHADE3,SHADE4,SHADE5,SHADE6,SHADE7,SHADE8,SHADE9,SHADE10,SHADE11,
//     SHADE12,SHADE13,SHADE14,SHADE15,SHADE16,SHADE17,SHADE18,SHADE19,SHADE20,SHADE21,SHADE22,SHADE23};
const char *colors[] = {RSHADE1,RSHADE2,RSHADE3,RSHADE4,RSHADE5,RSHADE6,RSHADE7};
const int c_len=sizeof colors/ sizeof*colors;
int main(){    
    float light[]={.5, 2,1.5};

    float module=sqrtf( light[X_]*light[X_]+light[Y_]*light[Y_]+light[Z_]*light[Z_]);
    light[X_]/=module;
    light[Y_]/=module;
    light[Z_]/=module;
    int n_row = 100, n_col = 100, total_len = n_row * n_col,
    r_count=0,r_color0=2;    
    short screen[total_len];

    float _near = .1f, _far = 100.f, _fov = 1.f / tanf(.35f * PI),aspec_ratio = (float)n_row / n_col,theta = 0;

    Display display={.near = _near, .far = _far, _fov,        
        .mx_proj = (aspec_ratio * _fov),
        .my_proj = _fov, 
        .mz_proj = _far / (_far - _near), 
        .mw_proj = -(_far * _near) / (_far - _near),
        .n_col=n_col,
        .n_row=n_row,
        .screen=screen};

    for (short *pointer = screen; pointer < screen + total_len; pointer++)
        *pointer = 0;

    float cube_verts[][3] = {
            {-.5f, -.5f, .5f}, {.5f, -.5f, .5f}, {.5f, .5f, .5f}, {-.5f, .5f, .5f}, 
            {-.5f, -.5f, -.5f}, {.5f, -.5f, -.5f}, {.5f, .5f, -.5f}, {-.5f, .5f, -.5f}};

    Object3D cube={
        .n_faces=6,.n_verts=8,
        .verts  =malloc(sizeof(void*)*8),
        .p_verts=malloc(sizeof(void*)*8),
        .t_verts=malloc(sizeof(void*)*8),
        .faces  =malloc(sizeof(void*)*6)};

    for (size_t i = 0; i < 8; i++)
        cube.verts[i]=cube_verts[i],
        cube.p_verts[i]=malloc(sizeof(float)*3),
        cube.t_verts[i]=malloc(sizeof(float)*3);

    float *faces[6][4] = {
            {cube.p_verts[0], cube.p_verts[1], cube.p_verts[2], cube.p_verts[3]}, 
            {cube.p_verts[1], cube.p_verts[5], cube.p_verts[6], cube.p_verts[2]}, 
            {cube.p_verts[5], cube.p_verts[4], cube.p_verts[7], cube.p_verts[6]}, 
            {cube.p_verts[4], cube.p_verts[0], cube.p_verts[3], cube.p_verts[7]}, 
            {cube.p_verts[3], cube.p_verts[2], cube.p_verts[6], cube.p_verts[7]}, 
            {cube.p_verts[4], cube.p_verts[5], cube.p_verts[1], cube.p_verts[0]}
    };

    for (size_t i = 0; i < 6; i++)
        cube.faces[i]=faces[i];

    int nverts=15;
    float 
        r1=.7f,r2=.35f,t=2*PI/nverts,tt=0,uu=0,
        *tfaces[nverts*nverts][4];

    Object3D torus={.n_verts=nverts*nverts,.n_faces=nverts*nverts,
        .verts  =malloc(sizeof(void*)*nverts*nverts),
        .p_verts=malloc(sizeof(void*)*nverts*nverts),
        .t_verts=malloc(sizeof(void*)*nverts*nverts),        
        .faces  =malloc(sizeof(void*)*nverts*nverts)};

    for (int i = 0; i < nverts; i++,tt+=t) 
        for (int j = 0,index=i*nverts; j < nverts; j++,uu+=t,index++){            
            torus.p_verts[index]=malloc(sizeof(float)*3);
            torus.t_verts[index]=malloc(sizeof(float)*3);        
            torus.verts  [index]=malloc(sizeof(float)*3);

            torus.verts  [index][X_]=(r1+r2*cosf(uu))*cosf(tt),
            torus.verts  [index][Y_]=(r1+r2*cosf(uu))*sinf(tt);
            torus.verts  [index][Z_]=r2*sinf(uu);
        }

    for (size_t row = 0; row < nverts; row++)
        for (size_t col = 0; col < nverts; col++)
            tfaces[CINDEX(row,col,nverts)][3]=torus.p_verts[CINDEX(row,col,nverts)],
            tfaces[CINDEX(row,col,nverts)][2]=torus.p_verts[CINDEX(row,(col+1)%nverts,nverts)],
            tfaces[CINDEX(row,col,nverts)][1]=torus.p_verts[CINDEX((row+1)%nverts,(col+1)%nverts,nverts)],
            tfaces[CINDEX(row,col,nverts)][0]=torus.p_verts[CINDEX((row+1)%nverts,col,nverts)];           
        
    for (size_t i = 0; i < nverts*nverts; i++)
        torus.faces[i]=tfaces[i];

    while (0X111C4E1){
        r_count=(++r_count%20);
        r_color0=r_count%20==0? rand()%6+1:r_color0;
        
        for (short *pointer = screen; pointer < screen + total_len; pointer++)
            *pointer =0;
        theta += .09;
        float sin_ = sinf(theta), cos_ = cosf(theta);        
        draw_object3d(&torus,&display,sin_,cos_,light,c_len);
        // draw_object3d(&cube,&display,sin_,cos_,light,c_len);
        for (size_t i = 0; i < total_len; i++)
            printf("%s", !(i % n_col) ? "\n" : colors[screen[i]]);
        usleep(130000);
        system("clear");
    }
}

/*_____________________________
  ________######_______________
  ____________#####____________
  ___##_________#####__________
  _####__________#####_________
  ############################_
  ############################_
  #####________######__________
  #####______######____________
  ________#######______________
  ________###__________________
  ________###__________________
  ________###__________________ É TUDO NOSSO NADA DELES!!!*/
