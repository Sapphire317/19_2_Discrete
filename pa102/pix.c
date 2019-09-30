#include <stdio.h>
#include <string.h>

void combinationUtil(int arr[], int data[], int start, int end,  
                     int index, int r, int k, int j, int N, int M); 

void printCombination(int arr[], int n, int r, int k, int j, int N, int M) 
{ 
    // A temporary array to store all combination one by one 
    int data[r]; 
  
    // Print all combination using temprary array 'data[]' 
    combinationUtil(arr, data, 0, n-1, 0, r, k, j, N, M); 
} 
  
/* arr[]  ---> Input Array 
   data[] ---> Temporary array to store current combination 
   start & end ---> Staring and Ending indexes in arr[] 
   index  ---> Current index in data[] 
   r ---> Size of a combination to be printed */
void combinationUtil(int arr[], int data[], int start, int end, 
                     int index, int r, int k, int j, int N, int M) 
{ 
    // Current combination is ready to be printed, print it
    int x[9]={0, 0, -1, -1, -1, 0, 1, 1, 1};
    int y[9]={0, -1, -1, 0, 1, 1, -1, 0, 1}; 
   
    if (index == r) {
        int isgood=1;
        int check[9]={0}; 
        for (int s=0; s<r; s++){
            check[data[s]]++;
        }

        for(int s=0; s<9; s++){
            if(check[s]!=0){
                if(!(k+x[s] <= N && k+x[s] >=1 && j+y[s] <=M && j+y[s] >= 1)){
                    isgood=0;
                }
            }
        }
        if(isgood){
             printf("(and ");  
         for(int m=0; m<9; m++){
             //If it is true
             if(check[m]!=0){
                 if(k+x[m] <= N && k+x[m] >=1 && j+y[m] <=M && j+y[m] >= 1){
                        printf("p%d%d ", k+x[m], j+y[m]);
                        // printf("(%d, %d)\n", i+x[m], j+y[m]);
                }
             }else{
                 if(k+x[m] <= N && k+x[m] >=1 && j+y[m] <=M && j+y[m] >= 1){
                        printf("(not p%d%d) ", k+x[m], j+y[m]);
                        // printf("(%d, %d)\n", i+x[m], j+y[m]);
                }
             }
                    
            }
            printf(")");
        }
       
        return; 
    } 
     
    // replace index with all possible elements. The condition 
    // "end-i+1 >= r-index" makes sure that including one element 
    // at index will make a combination with remaining elements 
    // at remaining positions 
  
    for (int i=start; i<=end && end-i+1 >= r-index; i++) 
    { 
        data[index] = arr[i]; 
        combinationUtil(arr, data, i+1, end, index+1, r, k, j, N, M); 
    }
     
} 




int main(){
    char arr[1001][1001];
    int i, j, m, n;
    int M=0;
    int N=0;

    char buf[1001]={};



    FILE *fp = fopen("p1", "r");
    
    if(fp==NULL) {
        perror("Error opening file");
        return -1;
    }

    while(fgets(buf, 1001, fp)){
        if(N==0){
            M=strlen(buf)/2;
        }
        for(int i=0; i<strlen(buf); i+=2){
            arr[N+1][i/2+1]=buf[i];
        }
        
        // printf("%lu\n", strlen(buf));
        N++;
    }

    // printf("M: %d, N: %d\n", M, N);


    // for(i=1; i<=N; i++){
    //     for(j=1; j<=M; j++){
    //         printf("%c ", arr[i][j]);
    //     }
    //     printf("\n");
    // }



    int x[9]={0, 0, -1, -1, -1, 0, 1, 1, 1};
    int y[9]={0, -1, -1, 0, 1, 1, -1, 0, 1};


    for(i=1; i<=N; i++){
        for(j=1; j<=M; j++){
            printf("(declare-const p%d%d Bool)\n", i, j);
        }
    }

    int ar[] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; 
    int r; 
    int s = sizeof(ar)/sizeof(ar[0]); 
   


    //(assert (and (or~~)))
    //Q

    printf("(assert (and ");
    for(i=1; i<=N; i++){
        for(j=1; j<=M; j++){
            if(arr[i][j]!='?'){
                printf("(or ");
                r=arr[i][j]-'0';
                 printCombination(ar, s, r, i, j, N, M); 
                printf(")");
            }
        }
    }
    printf("))\n");

    printf("(check-sat)\n(get-model)\n");

    fclose(fp);
    return 0;
}