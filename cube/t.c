#include <stdio.h>
#include <math.h>
#include <unistd.h>


int main() {
    for(int i=0;i<256;i++)
        printf("\033[48;5;%dm  %d\033[0m\n",i,i);
    	// printf("\033[0;0;%dm %d \n",i,i);
    
    return 0;
}