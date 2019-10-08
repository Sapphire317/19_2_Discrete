#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int row_num =1;
int col_num =1 ;
char Output[51][51];

int rowSum[51];
int colSum[51];

void receiveInputChar(int inputArray[][51]);

void receiveInputChar(int inputArray[][51]){
   
   char * lines = 0x0;
   size_t buf = 0;


    //First line will be the colSum.
    getline(&lines, &buf, stdin);
    int len = strlen(lines);
   int idx=1;
    for(int i = 0 ; i<len ; i++){
         if(idx==51) break;
         char c = lines[i];
            int element;
         if((c>=48 && c<= 57) || c==63){
                char num[10] ;
                num[0] = c;
                int j=0;
            while(lines[i+j+1]>=48 && lines[i+j+1]<= 57){
               j++;
               num[j] = lines[i+j];
            }
            num[j+1] = 0x0;
            i= i+j;
            element = atoi(num);
            colSum[idx++] = element;
         }
   }

    int col_len = idx-1;

   while(getline(&lines, &buf, stdin)!=EOF){
      int len = strlen(lines);
      int coldex=1;
      for(int i = 0 ; i<len ; i++){
         if(coldex==50) break;
         char c = lines[i];
            int element;
         if((c>=48 && c<= 57)){
                char num[10] ;
                num[0] = c;
                int j=0;
            while(lines[i+j+1]>=48 && lines[i+j+1]<= 57){
               j++;
               num[j] = lines[i+j];
            }
            num[j+1] = 0x0;
            i= i+j;
            element = atoi(num);
                if(coldex>col_len){
                    rowSum[row_num]=element; break;
                } 
            else inputArray[row_num][coldex++] = element;
         }
      }
      if(row_num==1) col_num = coldex;
      if(row_num==50){
         row_num++;
         break;
      }

        (coldex==col_num) ? row_num++ : 
         (coldex!=0) ? printf("please input correct grid, same column number before you enter\n") : 0x0;

   }
}

void printGrid(int inputArray[][51]){


    for(int i=1 ; i<=col_num-1 ; i++){
         printf("%d ",colSum[i]);
   }
    printf("\n");
   for(int i=1 ; i<= row_num-1 ; i++){
      for(int j=1 ; j<= col_num-1 ; j++){
         printf("%d ",inputArray[i][j]);   
      }
        printf("%d ", rowSum[i]);
      printf("\n");
   }


}

void printOnly(int inputArray[][51]){
    for(int i=1 ; i< row_num ; i++){
      for(int j=1 ; j< col_num ; j++){
         printf("%d ",inputArray[i][j]);   
      }
      printf("\n");
   }

}


void unique_combination(int l, int sum, int K, int local[], int A[], int idx, int sizeL, int sizeA, int rc, int rORc, FILE *fp){

    // rORc = 0  ===> col
    // rORc = 1  ===> row

    if(sum==K){
        int *temp;
        temp= (int*)malloc(sizeof(int)*(sizeA+1));
        for(int i=1; i<sizeA; i++) {temp[i]=0;}
         fprintf(fp, "(and ");

         
        for(int i=1; i <idx; i++){
            if(rORc) fprintf(fp, "(= r%dc%d 0) ", rc, local[i]);
            else  fprintf(fp, "(= r%dc%d 1) ", local[i], rc);

            temp[local[i]]++;
        }
        for(int i=1; i<sizeA; i++){
            if(temp[i]==0){
                if(rORc) fprintf(fp, "(= r%dc%d 1) ", rc, i);
                else fprintf(fp, "(= r%dc%d 0) ", i, rc);
            }
        }
        fprintf(fp, ")");
        return;
    }

    for(int i=l; i < sizeA; i++){
        if(sum + A[i] > K) continue;

        //if(i && A[i]==A[i-1] && i>2) continue;

        local[idx++]= i;

        unique_combination(i+1, sum + A[i], K, local, A, idx, sizeL, sizeA, rc, rORc,  fp);

        idx--;

    }
}

void Combination(int A[],  int K, int sizeA, int rc, int rORc, FILE* fp){
    int local[sizeA];
    int len = sizeof(local)/sizeof(int);

    unique_combination(1,0,K, local, A, 1, len, sizeA, rc, rORc, fp);
}
void basicCondition(int arr[][51], int N, int M, FILE* fp){


    /*

        M
    0 0 0 0 0 
    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0   N
    0 0 0 0 0
    0 0 0 0 0

    */

     for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            fprintf(fp, "(declare-const r%dc%d Int)\n", i, j);
        }
    }

    fprintf(fp, "(assert (and ");
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            fprintf(fp, "(and (> r%dc%d -1) (< r%dc%d 2))", i, j, i, j);
        }
    }
    fprintf(fp, "))\n");


    //Vertical SUM
    fprintf(fp, "(assert (and ");
    for(int m=1; m<=M; m++){
        int K = colSum[m];
        int *comb;
        comb=(int*)malloc(sizeof(int)*(N+1));
        for(int i=1; i<=N; i++){
            comb[i]=arr[i][m];
   
        }
    
    fprintf(fp, "(or ");
     Combination(comb, K, N+1, m, 0, fp);
    fprintf(fp, ")");
    }
     fprintf(fp, "))\n");

    //Horizontal SUM
     fprintf(fp, "(assert (and ");
    for(int n=1; n<=N; n++){
        int K = rowSum[n];
        int *comb;
        comb=(int*)malloc(sizeof(int)*(M+1));
        for(int i=1; i<=M; i++){
            comb[i]=arr[n][i];
        }
    
    fprintf(fp, "(or ");
     Combination(comb, K, M+1, n, 1, fp);
    fprintf(fp, ")");
    }
     fprintf(fp, "))\n");

   
     

 }

 int main(){
    int arr[51][51];
    int i, j, m, n;
    
    

     FILE * fp = fopen("./numCrossOutput","w");
    receiveInputChar(arr);
    //printGrid(arr);

    basicCondition(arr, row_num-1, col_num-1, fp);
    fprintf(fp, "(check-sat)\n(get-model)\n");
    fclose(fp);

    int solution[51][51];
    char filename[100];
    sprintf(filename, "z3 ./numCrossOutput > result");
    FILE * rs = popen(filename, "r");
    char buff[128];
    while(!feof(rs)){
            fscanf(rs, "%s", buff);
    }
    pclose(rs);

    sprintf(filename, "./result");
    FILE *z3 = fopen(filename, "r");
    char bury[128] ;
	char gi[128] ;
	char nv[128];
    int q, w, e;
    fscanf(z3, "%s", bury);
    if(bury[0]=='u' || bury[0]=='('){
        printf("No Solution\n");
		pclose(z3);
		return 0;
    }else{
        fscanf(z3, "%s",bury);
        for(int a=1; a<=(row_num-1) * (col_num-1); a++){
                fscanf(z3,"%s %s %s %s %s", bury, gi, bury, bury, nv);


                int glen = strlen(gi);
                int i, j;
                for( i = 0 ; i<glen ; i++){
                    if(gi[i] == 'c') break;
                }
                char subs[128]; char subs2[128];
                for(j=1; j<i; j++){
                    subs[j-1]=gi[j];
                }
                subs[j-1]=0x0;
                q = atoi(subs);

                j= i+1;
                int dex=0;
                while(gi[j]!=0x0){
                    subs2[dex++] = gi[j++];
                }
                subs2[dex]=0x0;
                w= atoi(subs2);

                if(strcmp(nv, "0)") != 0)
                    solution[q][w]=1;
                else
                    solution[q][w]=0;
        }

        //Print Solution 

        printf("\n\n");
        printf("========\n");
        printf("Solution\n");
        printf("========\n");
        for(int i=1; i<=row_num-1; i++){
            for(int j=1; j<=col_num-1; j++){
                printf("%d ", solution[i][j]);
            }
            printf("\n");
        }

        fclose(z3);
    }
    
    return 0;

}