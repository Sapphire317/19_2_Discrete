#include <stdio.h>
#include <string.h>

char board[1001][1001];

int main(){
    int i, j, k;

    char b[128];
    char s[128];
    char t[128];

    //Blow up the first and second line.
    scanf("%s %s", b, b);

    for(k=0; k < 25; k++){
        scanf("%s %s %s %s %s", b, s, b, b, t);

        i = s[1]-'0';
        j = s[2]-'0';
  
        if (strcmp(t, "0)") != 0){
            board[i][j]=1;
        }else{
            board[i][j]=0;
        }
    }
       
       for(i=1; i<=5; i++){
           for(j=1; j<=5; j++){
               printf("%d ", board[i][j]);
           }
           printf("\n");
       }

    
}