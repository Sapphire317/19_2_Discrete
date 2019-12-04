#include <stido.h>

int main(){
    int i, j, k, m ,n;
    int board[10][10];

    for(i=1; i<=10; i++){
        for(j=1; j<=10; j++){
            scanf("%d", &board[i][j]);
        }
    }


    for (i=1; i<=9; i++)
        for(j=1; j<=9; j++)
            printf("(declare-const p%d%d int)\n", i, j);

    
    //Q1
    printf("; Q1\n");
    printf("(assert (not ");
    for(i=1; i<=8; i++){
        printf("(and ");
        for(j=1; j <= 8; j++)
            printf("p%d%d ", i, j);
        printf(")") ;
    }
    printf("))\n");
}