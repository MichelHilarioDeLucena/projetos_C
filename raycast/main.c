#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>

#define BLACK "\033[0m  "
#define GRAY "\033[48;5;235m  "
#define RED "\033[48;5;196m  "
#define GREEN "\033[48;5;46m  "
#define YELLOW "\033[48;5;226m  "
#define BLUE "\033[48;5;21m  "
#define MAGENTA "\033[48;5;199m  "
#define CYAN "\033[48;5;45m  "
#define WHITE "\033[48;5;255m  "

#define PI 3.14159265358979323846

typedef struct point
{
    float x, y;
} point;
const int nr = 23, nc = 35;
char map[23][35] = {
    "**********************************",
    "*                                *",
    "*                      ****      *",
    "*                      ****      *",
    "*                      ****      *",
    "*                      ****      *",
    "*                                *",
    "*                                *",
    "*                                *",
    "*            ***                 *",
    "*            ***                 *",
    "*            ***                 *",
    "*                                *",
    "*                                *",
    "*                                *",
    "*                    ******      *",
    "*                    *           *",
    "*        +++         *           *",
    "*                    *           *",
    "*                                *",
    "*                                *",
    "*                                *",
    "**********************************"};
void raycast(point *p0, point *p1);
int main(){
    struct termios new_t, old_t;
    tcgetattr(STDIN_FILENO, &old_t);
    new_t = old_t;
    new_t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_t);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
    
    
    point player = {8, 15}, end = {0, 0};
    while(1){
        float theta = 0;
        for (size_t i = 0; i < nr; i++)
            for (size_t j = 0; j < nc-1; j++)
                if(map[i][j]=='+'||map[i][j]=='p')map[i][j]=' ';
                // map[i][j]=(map[i][j]=="+"||map[i][j]=="p")?' ':map[i][j];
        switch (getchar()){
            case 'w':player.y--;break;
            case 'a':player.x--;break;
            case 's':player.y++;break;
            case 'd':player.x++;break;     
        }
        while(getchar()!=EOF)getchar();
        
        
        for (float alpha = 0; alpha < 2 * PI; alpha += 0.01f, end.x = sinf(alpha) * 41, end.y = cosf(alpha) * 41)
            raycast(&player, &end);
            
        map[(int)player.y][(int)player.x]='p';
        for (size_t i = 0; i < nr; i++, puts(""))
            for (size_t j = 0; j < nc; j++)
                switch (map[i][j])
                {
                case '*':
                    printf("%s", BLUE);break;
                case 'p':
                    printf("%s", RED);break;
                case '+':
                    printf("%s", YELLOW);break;
                case ' ':
                    printf("%s", GRAY);break;
                default:
                    printf("%s", BLACK);
                }
        usleep(64000);
        system("clear");
    }

    tcsetattr(STDERR_FILENO, TCSANOW, &old_t);
}
void raycast(point *p0, point *p1)
{
    float dx = p1->x - p0->x, dy = p1->y - p0->y;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float ix = dx / steps,iy = dy / steps,x = p0->x, y = p0->y;
    
    for (int i = 0; i <= steps; i++, x += ix, y += iy){
        int kx=roundf(x),ky=roundf(y);   
        if(kx<0||ky<0||kx>=nc-1||ky>=nr)return;
        // printf("%d %d \n",kx,ky);
        if (map[(int)ky][(int)kx] != '*')
            map[(int)ky][(int)kx] = '+';
        else return;
    }
}