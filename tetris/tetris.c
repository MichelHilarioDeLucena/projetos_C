#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>

#define W_PANEL 21
#define H_PANEL 31
#define N_PIECES 7
#define N_POS_PIECES 4
#define ROW 0
#define COL 1
#define BLANK_PANEL ' '
#define MARKED_PANEL '#'
#define BLACK "\033[40m  "
#define RED "\033[41m  "
#define GREEN "\033[42m  "
#define YELLOW "\033[33;43m  "
#define BLUE "\033[44m  "
#define MAGENTA "\033[45m  "
#define CYAN "\033[46m  "
#define WHITE "\033[47m  "
#define SPEED 185000

void rotate(int *, int);
void input_logic(char, struct termios *, int[N_POS_PIECES][2], int *,char[H_PANEL][W_PANEL]);
void init_piece(int[N_POS_PIECES][2], int[2]);
int move_piece(char[H_PANEL][W_PANEL], int[N_POS_PIECES][2]);

int pieces[N_PIECES][N_POS_PIECES][2] = {{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
                                         {{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
                                         {{-1, 0}, {0, -1},{0, 0}, {1, 0}},
                                         {{-1, 0}, {0, 0}, {0, 1}, {1, 0}},
                                         {{-1, 0}, {-1, 1},{0, 0}, {0, 1}},
                                         {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},
                                         {{-1, 0}, {0, 0}, {1, 0}, {1, -1}}};



float dtime=SPEED,multi_speed=1;
int piece_index,color_index;
int curr_piece_pos[N_POS_PIECES][2];
char* currt_color[]={GREEN,BLUE,MAGENTA,CYAN,WHITE};
int main()
{
    srand(time(NULL));
    
    struct termios new_t, old_t;
    tcgetattr(STDIN_FILENO, &old_t);
    new_t = old_t;
    new_t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_t);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

    clock_t start,end;

    int piece_pos[N_POS_PIECES][2];
    int offset_piece[2];
    int score = 0;
    char panel[H_PANEL][W_PANEL];

    init_piece(piece_pos, offset_piece);

    for (int i = 0; i < H_PANEL; i++)
        for (int j = 0; j < W_PANEL - 1; j++)
            panel[i][j] = i * j != 0 && i < H_PANEL - 1 && j < W_PANEL - 2 ? BLANK_PANEL : MARKED_PANEL;
    while (1)
    {
        start=clock();
        input_logic(getchar(), &old_t, piece_pos, offset_piece,panel);
        while(getchar()!=EOF)getchar();
        puts("  TETRIS simples");
        printf("\nscore:%d\n",score);

        offset_piece[ROW]++;
        int collided = move_piece(panel, piece_pos);

        for (int j = 0; j < N_POS_PIECES; j++)
            panel[piece_pos[j][ROW]][piece_pos[j][COL]] = '0';
        for (int i = 0; i < H_PANEL; i++){            
            for (int j = 0; j < W_PANEL - 1; j++)
                printf("%s", 
                panel[i][j]==MARKED_PANEL?RED:panel[i][j]=='0'? currt_color[color_index]:YELLOW);                
            puts("\033[0m");
        }
        for (int j = 0; j < N_POS_PIECES; j++)
            panel[piece_pos[j][ROW]][piece_pos[j][COL]] = MARKED_PANEL;
        // for (int i = 0; i < H_PANEL; i++)
        //     printf("%.20s\n", panel[i]);

        if (!collided)
            for (int j = 0; j < N_POS_PIECES; j++)
                panel[piece_pos[j][ROW]][piece_pos[j][COL]] = BLANK_PANEL;
        else
        {
            for (int row = 1; row < H_PANEL - 1; row++){
                int marked=0;
                for (int col = 1l; col < W_PANEL - 2; col++)
                    marked+=panel[row][col] != BLANK_PANEL;
                if(marked&&row==2){
                    system("clear");
                    puts("FIM DE JOGO\nperdeu playboy");
                    tcsetattr(STDERR_FILENO, TCSANOW, &old_t);
                    exit(0);
                }
                if(marked==W_PANEL-3){
                    for (int i = H_PANEL-2; i > 1; i--)
                        for (int col_ = 1; col_< W_PANEL - 2; col_++)
                            panel[i][col_] = panel[i-1][col_],
                            panel[i-1][col_] = BLANK_PANEL;
                    score++;
                }
            }
            init_piece(piece_pos, offset_piece);
        }
        end=(clock()-start)*10;
        // usleep(dtime*multi_speed);
        usleep(dtime-end);
        system("clear");
    }
    tcsetattr(STDERR_FILENO, TCSANOW, &old_t);
}

void rotate(int *p, int r)
{
    int aux = p[r];
    p[r] = p[!r];
    p[!r] = -aux;
}

void input_logic(char c, struct termios *old_t, int piece[N_POS_PIECES][2], int *offset,char panel[H_PANEL][W_PANEL])
{
    switch (c)
    {
    case 'q':
        for (int i = 0; i < N_POS_PIECES; i++)
        {
            rotate(curr_piece_pos[i], 0);
            
            piece[i][ROW] = offset[ROW] + curr_piece_pos[i][ROW];
            piece[i][COL] = offset[COL] + curr_piece_pos[i][COL];
        }
        break;
    case 'e':
        for (int i = 0; i < N_PIECES; i++)
        {
            rotate(curr_piece_pos[i], 1);
            piece[i][ROW] = offset[ROW] + curr_piece_pos[i][ROW];
            piece[i][COL] = offset[COL] + curr_piece_pos[i][COL];
        }
        break;
    case 'a':
        for (int j = 0; j < N_POS_PIECES; j++)
            if(panel[piece[j][ROW]][piece[j][COL]-1]==MARKED_PANEL)
                return;
        for (int j = 0; j < N_POS_PIECES; j++)
            offset[COL]=--piece[j][COL];
        break;

    case 'd':
        for (int j = 0; j < N_POS_PIECES; j++)
            if(panel[piece[j][ROW]][piece[j][COL]+1]==MARKED_PANEL)
                return;
        for (int j = 0; j < N_POS_PIECES; j++){
            offset[COL]= ++piece[j][COL];
        }
        break;
    case 's':
        // dtime=dtime==1?.1:1;
    break;
    case '1':
        puts("\033[0m");
        tcsetattr(STDERR_FILENO, TCSANOW, old_t);
        exit(EXIT_SUCCESS);
    }
    
}

void init_piece(int p[N_POS_PIECES][2], int offset[2])
{
    offset[ROW] = 1, offset[COL] = 9;
    piece_index = rand() % N_PIECES,color_index=rand()%4;
    for (int i = 0; i < N_POS_PIECES; i++)
        p[i][ROW] = (curr_piece_pos[i][ROW] = pieces[piece_index][i][ROW]) + offset[ROW],
        p[i][COL] = (curr_piece_pos[i][COL] = pieces[piece_index][i][COL]) + offset[COL];
}

int move_piece(char panel[H_PANEL][W_PANEL], int p[N_POS_PIECES][2])
{
    for (int i = 0; i < N_POS_PIECES; i++)
        if (panel[p[i][ROW] + 1][p[i][COL]] == MARKED_PANEL)
            return 1;
    for (int i = 0; i < N_POS_PIECES; i++)
        p[i][ROW]++;
    return 0;
}
