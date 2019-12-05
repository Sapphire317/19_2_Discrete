#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <glib.h>
#include <math.h>
#include "../include/libstemmer.h"

#define BUFSIZE 1024
#define K 1000

//Make expression to lower case
void tolow(char* expr, int len);

//Exclude all letters except alphabet.
void exclude(char* expr, int len);

//Retrieve stop words to stop word list.
void bringStopwords();

//Check if the word is in stop word list.
int isInList(char* word);

//Print stop word list.
void printSwlist();

//The double value of rescaled word list.
double rescale_val(char** wordList, int len);

//Calculate Bayes Probability
double bayes_prob(char** wordList);

//Push data to the hash table.
void build_table(char* msg, GHashTable* counter);


char swlist[BUFSIZE][BUFSIZE];
int main(){

    //Make new hash table for Negative Sets and Non-negative sets.
	GHashTable* wordList = g_hash_table_new(g_str_hash, g_str_equal) ;

    char msg[BUFSIZE];
    double prob_neg = 0.0;
    double prob_n_neg = 0.0;
    double bayes_prob;

    

    printf("Enter the reply you want to check : "); 
    gets(msg);


    build_table(msg, wordList);

    GHashTableIter iter;

    gpointer* val;
    gpointer* key;

    g_hash_table_iter_init (&iter, wordList);
    while (g_hash_table_iter_next (&iter, &key, &val)){
    
        char* t = key;
        
        FILE* fp = fopen("model.csv", "r");
        char * line = 0x0 ;
        size_t r ; 
        size_t n = 0 ;
        while (getline(&line, &n, fp) >= 0) {
            char * w ;
		    char * _line = line ;

            w=strtok(line, "|");

            //If it is in model.csv
            if(strcmp(w, t)==0){
                char* ptr;
                w=strtok(NULL, "|");
                prob_neg+=log10(strtod(w, &ptr));
    
                w=strtok(NULL, "|");
                prob_n_neg+=log10(strtod(w, &ptr));

                //printf("prob_neg : %lf\n", prob_neg);
                //printf("prob_n_neg : %lf\n", prob_n_neg);
            }

        }


    }

    //Rescaling
    prob_neg = pow(10, prob_neg);
    prob_n_neg = pow(10, prob_n_neg);

    bayes_prob = (prob_neg)/(prob_neg + prob_n_neg);

    printf("Probability: %lf\n", bayes_prob);
   


}





void tolow(char* expr, int len){
    for(int i=0; i<len; i++){
        expr[i]=tolower(expr[i]);
    }
}

void exclude(char* expr, int len){
    int i, j;
    for(i=0; i<len; i++){
        while(!((expr[i]>='a' && expr[i]<='z') || ((expr[i]>='A' && expr[i]<='Z') || expr[i]=='\0') || expr[i]==' ')){
            for(j=i; j<len; j++){
                expr[j]=expr[j+1];
            }
            expr[j]='\0';
        }
    }
}
void build_table(char* msg, GHashTable* counter){
        //Declare varliables for stemmer
    struct sb_stemmer* stemmer ;
	stemmer = sb_stemmer_new("english", 0x0) ;
	char* s;
    char buf[1024];

    //Bring stopwords.
    bringStopwords();

    exclude(msg, strlen(msg));
    tolow(msg, strlen(msg));


    char*pch;
    pch = strtok(msg, " \n");
    do{

        if(!isInList(pch)){
                s = sb_stemmer_stem(stemmer, pch, strlen(pch)) ;
			int * d ;			
			d = g_hash_table_lookup(counter, s) ;
			if (d == NULL) {
				d = malloc(sizeof(int)) ;
				*d = 1 ;
				g_hash_table_insert(counter, strdup(s), d) ;
			}
			else {
				*d = *d + 1 ;
			}
         }

         pch=strtok(NULL, " \n");
    }while(pch != NULL);
            
		

}

int isInList(char* word){
    
    for(int i=0; i<127; i++){
        if(strcmp(word, swlist[i])==0)
            return 1;
    }
    
    return 0;
}

void bringStopwords(){
    FILE* fp;
    char buf[BUFSIZE];

    //File open.
    fp = fopen("stopwords.txt", "r");
    //File open check.
    if(!fp){
        printf("File open error!\n");
    }
    //Move words to stop word list.
    int i=0;
    while(fgets(buf, BUFSIZE, fp) != NULL){
        size_t ln =strlen(buf)-1;
        if(*buf && buf[ln]=='\n') buf[ln]='\0';
        strcpy(swlist[i], buf); i++;
    }
    //File close.
    fclose(fp);

}