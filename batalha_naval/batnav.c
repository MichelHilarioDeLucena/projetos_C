#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#define LEN_TABLE 10

void print_table(char [][LEN_TABLE],int);
void auto_set_piece(int ,char [][LEN_TABLE]);
void manual_set_piece(int ,char [][LEN_TABLE]);
short is_positionable(int ,char [][LEN_TABLE],short,short,short);
short check_hit(short,short,char [][LEN_TABLE]);

int main(){
    srand(time(NULL));
    char player_table[LEN_TABLE][LEN_TABLE];
    char bot_table[LEN_TABLE][LEN_TABLE];

    for (int i = 0; i < LEN_TABLE; i++)    
        for (int j = 0; j < LEN_TABLE; j++){
            bot_table[i][j]=player_table[i][j]='_';
        }
    puts("\n******* BATALHA NAVAL v0.01 *******\n\n\n\n\n");
    puts("digite 'y' para iniciar ou qualquer outra tecla para sair");
    
    if(getchar()=='y'){
        puts("\n\n\n**** deseja posicionar suas peças manualmente ? sim(y) ou não (any)\n");
        while (getchar()!='\n');
        if(getchar()=='y')
            for(int p=2;p<7;p++)
                manual_set_piece(p,player_table);
        else
            for(int p=2;p<7;p++)
                auto_set_piece(p,player_table);
        for(int p=2;p<7;p++)
            auto_set_piece(p,bot_table);
        int run=1;
        printf("\n\n\n\n       COMEÇOU!\n\n");
        while(run){
            short row,col;
            
            puts("\ntabela do jogador");
            print_table(player_table,0);

            puts("\ntabela do adversario(BOT)");
            print_table(bot_table,1);           

            puts("\nescolha uma fileira de 0 a 9");
            scanf("%hd",&row);
            while(1)
                if(row<0||row>=LEN_TABLE)
                    puts("coordenada invalida");
                else break;
            puts("\nescolha uma coluna de 0 a 9");            
            scanf("%hd",&col);
            while(1)
                if(col<0||col>=LEN_TABLE)
                    puts("coordenada invalida");
                else break;

            short hit=check_hit(row,col,bot_table);
            puts(hit ? "-ataque bem sucedido":"-ataque fracassado");

            hit=check_hit(rand()%LEN_TABLE,rand()%LEN_TABLE,player_table);            
            puts(hit ? "-ataque inimigo bem sucedido":"-ataque inigimo fracassado");
        }
    }
    return 0;    
}

void print_table(char t[][LEN_TABLE],int hide){
    printf("\n_|0|1|2|3|4|5|6|7|8|9|");
    for (int i = 0; i < LEN_TABLE; i++){
        printf("\n%d|",i);
        for (int j = 0; j < LEN_TABLE; j++)
            printf("%c|",hide && t[i][j]!='E'&&t[i][j]!='x'?'_':t[i][j]);
    }
}

short check_hit(short r,short c,char t[][LEN_TABLE]){
    short is_hit=t[r][c]!='_';
    t[r][c]=is_hit?'x':'E';
    return is_hit;
}

short is_positionable(int p,char t[][LEN_TABLE],short ortn,short r,short c){
    for (int i = 0; i < p; i++){
        if(t[r][c]!='_'||r>=LEN_TABLE||c>=LEN_TABLE) return 0;
        r+=ortn;
        c+=!ortn;
    }
    return 1;
}

void manual_set_piece(int p,char t[][LEN_TABLE]){
    short row,col,ortn;
    printf("\npeça de tamanho:%d\n",p);
    print_table(t,0);
    puts("--------------------");
    do{
        puts("escolha uma fileira de 0 a 9");
        scanf("%hd",&row);
        puts("escolha uma coluna de 0 a 9");
        scanf("%hd",&col);
        puts("escolha uma orientacao vertical(1) ou horizontal(0)");
        scanf("%hd",&ortn);
        printf("\n\nr:%d c:%d o:%d\n",row,col,ortn);
    }while(!is_positionable(p,t,ortn,row,col));

    for(int i=0;i<p;i++){
        t[row][col]='0'+p;
        row+=ortn;
        col+=!ortn;
    }
    puts("\nresultado:\n");
    print_table(t,1);
    puts("----------------------------\n");
}

void auto_set_piece(int p,char t[][LEN_TABLE]){
    short r_row,r_col,ortn;
    
    do{
        r_row=rand()%(LEN_TABLE);
        if(r_row<LEN_TABLE-p){
            r_col=rand()%LEN_TABLE;
            if(r_col<LEN_TABLE-p)
                ortn=rand()%2;       
            else
                ortn=0;     
        }else{
            ortn=1;
            r_col=rand()%(LEN_TABLE-p);
        }
        
    }while(!is_positionable(p,t,ortn,r_row,r_col));

    for(int i=0;i<p;i++){
        t[r_row][r_col]='0'+p;
        r_row+=ortn;
        r_col+=!ortn;
    }
}