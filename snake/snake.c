#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#define LEN_PANEL 21

enum {VOID_PANEL='_',SNAKE='#',APPLE='*'}PANEL_TYPE;

void config_terminal(int fd,struct termios* oldT){
    struct termios newT;
    tcgetattr(fd,oldT);
    newT=*oldT;
    newT.c_lflag &=~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&newT);
}

void generate_apple(char p[][LEN_PANEL]){
    int r,c;
    do{
        r=rand()%(LEN_PANEL-1);
        c=rand()%(LEN_PANEL-1);        
    }while(p[r][c]!=VOID_PANEL);
    p[r][c]=APPLE;
}

int main(){
    srand(time(NULL));

    struct termios def_termios;    
    int fd=STDIN_FILENO;
    config_terminal(fd,&def_termios);
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL)|O_NONBLOCK);
    
    char panel[LEN_PANEL][LEN_PANEL];
    char snake [LEN_PANEL*LEN_PANEL][2]={};
    int snake_index=3;

    for (int i = 0; i < LEN_PANEL*LEN_PANEL; i++)
        snake[i][0]=snake[i][1]=0;
    puts("");

    for (int i = 0; i < LEN_PANEL; i++){
        for (int j = 0; j < LEN_PANEL; j++)
            panel[i][j]=VOID_PANEL;
        //panel[i][LEN_PANEL-1]='\n';
    }
    
    int d_row=0,d_col=1;
    panel[snake[0][0]][snake[0][1]]=SNAKE;
    system("clear");

    generate_apple(panel);
    while(1){         
        switch (getchar()){
        case 'w':d_row=-1,d_col= 0;break;
        case 'a':d_row= 0,d_col=-1;break;
        case 's':d_row= 1,d_col= 0;break;
        case 'd':d_row= 0,d_col= 1;break;
        case 'q':
            tcsetattr(STDERR_FILENO,TCSANOW,&def_termios);
            return 0;
        }
        
        panel[snake[snake_index-1][0]][snake[snake_index-1][1]]=VOID_PANEL;

        for (size_t i = snake_index-1; i >0; i--){
            snake[i][0]=snake[i-1][0];
            snake[i][1]=snake[i-1][1];
        }
        short row=snake[0][0]+d_row,col=snake[0][1]+d_col;
        if(panel[row][col]==SNAKE||!(row>=0&&row<LEN_PANEL && col>=0&&col<LEN_PANEL-1)){
            system("clear");
            puts("O JOGO ACABOU.");
            tcsetattr(STDERR_FILENO,TCSANOW,&def_termios);
            return 1;
        }else if(panel[row][col]==APPLE){
            snake_index++;
            generate_apple(panel);
        }

        panel[snake[0][0]=row][snake[0][1]=col]=SNAKE;
        //update(panel); 
        printf("pontos:%d\npressione q para terminar o jogo\n",snake_index-3);
        for (int i = 0; i < LEN_PANEL; i++){
            for (int j = 0; j < LEN_PANEL; j++)
                printf(panel[i][j]==SNAKE?"\033[42m  ":panel[i][j]==APPLE?"\033[41m  ":"\033[47m  ");
            puts("\033[40m");
        }
        //puts(panel[0]);
        usleep(150000);
        system("clear");
    }
    tcsetattr(STDERR_FILENO,TCSANOW,&def_termios);
    
}