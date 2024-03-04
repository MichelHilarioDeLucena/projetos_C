
#include "my_graphics.h"

void draw_object3d(Object3D *obj,Display *display,float sin_,float cos_,float *light){
    //printf("%d\n",obj->n_verts);
    for (size_t i = 0; i < obj->n_verts; i++){
            obj->t_verts[i][X_] = obj->verts[i][X_]* cos_ + obj->verts[i][Z_] * sin_;
            obj->t_verts[i][Y_] = obj->verts[i][X_]* sin_ * sin_  + cos_ * obj->verts[i][Y_] + obj->verts[i][Z_] *-sin_ * cos_ ;
            obj->t_verts[i][Z_] = obj->verts[i][X_]*-cos_ * sin_  + sin_ * obj->verts[i][Y_] + obj->verts[i][Z_] * cos_ * cos_;

            obj->t_verts[i][Z_]+=5.f;
            float
                px = obj->t_verts[i][X_] * display->mx_proj,
                py = obj->t_verts[i][Y_] * display->my_proj,
                pz = obj->t_verts[i][Z_] * display->mz_proj,
                pw = display->mw_proj + obj->t_verts[i][Z_];

            obj->p_verts[i][X_] = (px * display->n_col / pw) + 55;
            obj->p_verts[i][Y_] = (py * display->n_row / pw) + 55;
            obj->p_verts[i][Z_] = (pz * display->n_row / pw) + 55;
        }
        if(!obj->faces)return;
        
        for (int k = obj->n_faces-1; k >= 0;k--){
            int q_i=((size_t)obj->faces[k][1]-(size_t)obj->p_verts)/0x40-0x3;
            int r_i=((size_t)obj->faces[k][0]-(size_t)obj->p_verts)/0x40-0x3;
            int s_i=((size_t)obj->faces[k][3]-(size_t)obj->p_verts)/0x40-0x3;            
            float
                v2[]={
                    obj->p_verts[q_i][X_]-obj->p_verts[r_i][X_],
                    obj->p_verts[q_i][Y_]-obj->p_verts[r_i][Y_],
                    obj->p_verts[q_i][Z_]-obj->p_verts[r_i][Z_]
                },
                v1[]={
                    obj->p_verts[s_i][X_]-obj->p_verts[r_i][X_],
                    obj->p_verts[s_i][Y_]-obj->p_verts[r_i][Y_],
                    obj->p_verts[s_i][Z_]-obj->p_verts[r_i][Z_]
                };
            float normal_vec[]={
                v1[Y_]*v2[Z_]-v2[Y_]*v1[Z_],
              -(v1[X_]*v2[Z_]-v2[X_]*v1[Z_]),
                v1[X_]*v2[Y_]-v2[X_]*v1[Y_]
            };
            
            if(-normal_vec[Z_]>0.f)continue;
            float module=sqrtf( normal_vec[X_]*normal_vec[X_]+
                                normal_vec[Y_]*normal_vec[Y_]+
                                normal_vec[Z_]*normal_vec[Z_]);
            normal_vec[X_]/=module;
            normal_vec[Y_]/=module;
            normal_vec[Z_]/=module;
            float dp=24.f*(normal_vec[X_]*light[X_] + normal_vec[Y_]*light[Y_] + normal_vec[Z_]*light[Z_]);            
            fill_rect(display->screen, obj->faces[k],dp>0?dp:0);
            // fill_rect(screen, faces[k],24+k);            
            // draw_line(n_col, screen, faces[k][0], faces[k][1],k+24);
            // draw_line(n_col, screen, faces[k][0], faces[k][3],k+24);
            // draw_line(n_col, screen, faces[k][2], faces[k][3],k+24);
            // draw_line(n_col, screen, faces[k][2], faces[k][1],k+24); 
        }
}


void draw_line(int m_col, short *scr, float p0[2], float p1[2], int value)
{
    float dx = p1[X_] - p0[X_], dy = p1[Y_] - p0[Y_];
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float ix = dx / steps;
    float iy = dy / steps;
    float x = p0[X_], y = p0[Y_];
    for (int i = 0; i <= steps; i++)
        scr[(int)roundf(x) * m_col + (int)roundf(y)] = value, x += ix, y += iy;
}

void draw_poly2D(int w, short *pnl, int n_vert, float poly[n_vert][2], int start)
{
    for (int point = start + 1; point < n_vert; point++)
        draw_line(w, pnl, poly[point - 1], poly[point], 1);
    draw_line(w, pnl, poly[n_vert - 1], poly[start], 1);
}

int onLine(float l1[2][2], float p[2])
{
    if (p[X_] <= fmax(l1[0][X_], l1[1][X_]) && p[X_] >= fmin(l1[0][X_], l1[1][X_]) && (p[Y_] <= fmax(l1[0][Y_], l1[1][Y_]) && p[Y_] >= fmin(l1[0][Y_], l1[1][Y_])))
        return 1;
    return 0;
}
int direction(float a[2], float b[2], float c[2])
{
    float val = (b[Y_] - a[Y_]) * (c[X_] - b[X_]) - (b[X_] - a[X_]) * (c[Y_] - b[Y_]);
    return val == 0 ? 0 : val < 0 ? 2: 1;
}

int isIntersect(float l1[2][2], float l2[2][2])
{
    int dir1 = direction(l1[0], l1[1], l2[0]);
    int dir2 = direction(l1[0], l1[1], l2[1]);
    int dir3 = direction(l2[0], l2[1], l1[0]);
    int dir4 = direction(l2[0], l2[1], l1[1]);

    return (dir1 != dir2 && dir3 != dir4)   ^
           (dir1 == 0 && onLine(l1, l2[0])) ^
           (dir2 == 0 && onLine(l1, l2[1])) ^
           (dir3 == 0 && onLine(l2, l1[0])) ^
           (dir4 == 0 && onLine(l2, l1[1]));
}
int checkInside(float **poly, int n, float p[2])
{
    if (n < 3)
        return 0;
    float exline[][2] = {{p[0], p[1]}, {9999, p[0]}};
    int count = 0;
    int i = 0;
    do
    {
        float side[2][2] = {{poly[i][0], poly[i][1]}, poly[(i + 1) % n][0], poly[(i + 1) % n][1]};
        if (isIntersect(side, exline))
        {
            if (!direction(side[0], p, side[1]))
                return onLine(side, p);
            count++;
        }
        i = (i + 1) % n;
    } while (i != 0);
    return count & 1;
}
void fill_rect(short *scr, float **rect, int shade)
{
    int
        max_x = fmaxf(fmaxf(fmaxf(rect[0][X_], rect[1][X_]), rect[2][X_]), rect[3][X_]),
        min_x = fminf(fminf(fminf(rect[0][X_], rect[1][X_]), rect[2][X_]), rect[3][X_]),

        max_y = fmaxf(fmaxf(fmaxf(rect[0][Y_], rect[1][Y_]), rect[2][Y_]), rect[3][Y_]),
        min_y = fminf(fminf(fminf(rect[0][Y_], rect[1][Y_]), rect[2][Y_]), rect[3][Y_]);

    for (int i = min_y; i <= max_y; i++)
        for (int j = min_x; j <= max_x; j++)
            if (checkInside(rect, 4, (float[]){j, i}))
                scr[(int)roundf(j) * 100 + (int)roundf(i)] = shade;
}