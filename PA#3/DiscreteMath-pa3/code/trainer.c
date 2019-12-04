#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <glib.h>
#include <math.h>
#include "../include/libstemmer.h"


#define BUFSIZE  1024
#define NUM_TRAIN_NEG 9079
#define NUM_TRAIN_NONNEG 5565
#define NUM_TEST_NEG 100
#define NUM_TEST_NONNEG 100
#define K 10


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

//Print word from hash table.
void print_counter (gpointer key, gpointer value, gpointer userdata);

void create_model(FILE *fp, GHashTable* counter);

void add_word(gpointer key, gpointer value, gpointer fp);

//The double value of smoothed possibility with k
double smooth_val(int k, char* word, GHashTable* counter);

//The double value of rescaled word list.
double rescale_val(char** wordList, int len, GHashTable* counter);

//Push data to the hash table.
void build_table(FILE *fp, GHashTable* counter);

//Calculate Bayes Probability
double bayes_prob(char** wordList,  GHashTable* negC, GHashTable* n_negC);


char swlist[BUFSIZE][BUFSIZE];


int main(){
    //Open the file.
   
    FILE * f = fopen("../data/train.negative.csv", "r") ;
    FILE * f2 = fopen("../data/train.non-negative.csv", "r");

    //Make new hash table for Negative Sets and Non-negative sets.
	GHashTable* negC = g_hash_table_new(g_str_hash, g_str_equal) ;
    GHashTable* n_negC = g_hash_table_new(g_str_hash, g_str_equal);

    build_table(f, negC);
    build_table(f2, n_negC);
    

    // double prob_neg = pow(10, rescale_val(wordList, 3, negC));
    // double prob_n_neg = pow(10, rescale_val(wordList, 3, n_negC));
    //  char *pos_wordList[100] = {"very", "fast", "thanx"};
    //  char *neg_wordList[100] = {"late", "worst", "delay"};

    FILE *fp = fopen("model.csv", "w");

    GHashTableIter iter;

    gpointer* val;
    gpointer* key;

    // My problem is in the following loop
    // it always returns the same and the last key value pair

    g_hash_table_iter_init (&iter, negC);
    while (g_hash_table_iter_next (&iter, &key, &val)){
        char buff[100];
        char* t = key;
        sprintf(buff, "%s|%lf|%lf\n", t, smooth_val(K, t, negC), smooth_val(K, t, n_negC));
        fputs(buff, fp);
    }
    fclose(fp);


    
    //printf("Probability of Positive List : %lf\n", bayes_prob(pos_wordList, negC, n_negC));
    //printf("Probability of Negative List : %lf\n", bayes_prob(neg_wordList, negC, n_negC));
    //Print all words with its frequency
    //g_hash_table_foreach(n_negC, print_counter, 0x0) ;
	

	fclose(f) ;
    fclose(f2);

}



double bayes_prob(char** wordList, GHashTable* negC, GHashTable* n_negC){
    double prob_neg = pow(10, rescale_val(wordList, 3, negC));
    double prob_n_neg = pow(10, rescale_val(wordList, 3, n_negC));
    // printf("%lf\n", pow(10, rescale_val(wordList, 3, negC)));
    // printf("%lf\n", pow(10, rescale_val(wordList, 3, n_negC)));

    double bayes_prob = (prob_neg)/(prob_neg + prob_n_neg);
    return bayes_prob;
}

void build_table(FILE *f, GHashTable* counter){
        //Declare varliables for stemmer
    struct sb_stemmer* stemmer ;
	stemmer = sb_stemmer_new("english", 0x0) ;
	char* s;
    char buf[1024];

    //Bring stopwords.
    bringStopwords();

	char * line = 0x0 ;
	size_t r ; 
	size_t n = 0 ;

    //Read line by line.
	while (getline(&line, &n, f) >= 0) {
		char * t ;
		char * _line = line ;

        exclude(line, strlen(line));
        tolow(line, strlen(line));

		for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {
            
            if(!isInList(t)){
                s = sb_stemmer_stem(stemmer, t, strlen(t)) ;
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
		}
		free(_line) ;
		line = 0x0 ;
	}

}

double smooth_val(int k, char* word, GHashTable* counter){

    double freq;
    if(g_hash_table_contains(counter, word)){
        freq =  *((int *) g_hash_table_lookup(counter, word));
    }else{
        freq = 0.0;
    }
    

    //printf("smooth val : %lf\n", ((k + freq)/(2*k + NUM_TRAIN_NEG)));
    return (k + freq)/(2*k + g_hash_table_size(counter));
}

double rescale_val(char** wordList, int len, GHashTable* counter){
    double final_val = 0.0;
    double temp_val;
    int i;
    for(i=0; i<len; i++){
        //printf("final_val :%lf\n", final_val);
        final_val+=log10(smooth_val(K, wordList[i], counter));
    }

    return final_val;
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

int isInList(char* word){
    
    for(int i=0; i<127; i++){
        if(strcmp(word, swlist[i])==0)
            return 1;
    }
    
    return 0;
}

void printSwlist(){
    for(int i=0; i<BUFSIZE; i++){
        printf("%s", swlist[i]);
    }
}

void print_counter (gpointer key, gpointer value, gpointer userdata) 
{
	char * t = key ;
	int * d = value ;

	printf("(%s, %d)\n", t, *d) ;
}
