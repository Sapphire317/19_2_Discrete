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


    for(int i=1 ; i<row_num ; i++){
			printf("%d ",colSum[i]);
	}
    printf("\n");
	for(int i=1 ; i< row_num ; i++){
		for(int j=1 ; j< col_num ; j++){
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


void basicCondition(int arr[][51], int N, int M, FILE* fp){
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
    

    int ar[] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; 
    int r; 
    int s = sizeof(ar)/sizeof(ar[0]); 
   
    //Q
    fprintf(fp, "(assert (and ");
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            if(arr[i][j]!='?'){
                fprintf(fp, "(or ");
                r=arr[i][j]-'0';
                // r= the number of center
                // i, j = the x, y coordination 
                 printCombination(ar, s, r, i, j, N, M, fp); 
                fprintf(fp, ")");
            }
        }
    }
    fprintf(fp, "))\n");
}




int main(){
    int arr[51][51];
    int i, j, m, n;
    

     FILE * fp = fopen("./numCrossOutput","w");
    receiveInputChar(arr);
    fprintf(fp, "(check-sat)\n(get-model)\n");

    fclose(fp);
    // printOnly(arr);
    return 0;

}