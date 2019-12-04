#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <glib.h>
#include "../include/libstemmer.h"


#define MAX 1024
#define NUM_TRAIN_NEG 9079
#define NUM_TRAIN_NONNEG 5565
#define NUM_TEST_NEG 100
#define NUM_TEST_NONNEG 100


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

void print_counter (gpointer key, gpointer value, gpointer userdata);
char swlist[MAX][MAX];

struct wordDic {
    char word[MAX][MAX];
    int count[MAX];
    int total;
};

//Reduce dictionary
void reduceDic(char* expr, int len, struct wordDic *wd);

//Print stop word list.
void printDic(struct wordDic *wd);

void print_counter (gpointer key, gpointer value, gpointer userdata) 
{
	char * t = key ;
	int * d = value ;

	printf("(%s, %d)\n", t, *d) ;
}


int main(){

    //Open the file.
    FILE * f = fopen("../data/train.negative.csv", "r") ;

    //Make new hash table.
	GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal) ;

    //Declare varliables for stemmer
    struct sb_stemmer * stemmer ;
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

    //Print all words with its frequency
	g_hash_table_foreach(counter, print_counter, 0x0) ;

	printf("worst: %d\n", *((int *) g_hash_table_lookup(counter, "worst"))) ;

	fclose(f) ;
    

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
    char buf[MAX];

    //File open.
    fp = fopen("stopwords.txt", "r");
    //File open check.
    if(!fp){
        printf("File open error!\n");
    }
    //Move words to stop word list.
    int i=0;
    while(fgets(buf, MAX, fp) != NULL){
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

void reduceDic(char* expr, int len, struct wordDic *wd){

    struct sb_stemmer * stemmer ;
	stemmer = sb_stemmer_new("english", 0x0) ;
	char* s;
	
    
    //Remove stopwords
    int wordIdx, cntIdx = 0;
    const char *pch;
    char buf[1024];
    pch = strtok(expr, " ");
    strcpy(buf, pch);
    s = sb_stemmer_stem(stemmer, buf, strlen(buf)) ;
    if(!isInList(s)){
        strcpy(wd->word[wd->total], s);
        wd->count[wd->total]++;
        wd->total++;
    }
    pch = strtok(NULL, " ");
    strcpy(buf, pch);
    s = sb_stemmer_stem(stemmer, buf, strlen(buf)) ;
    while(pch != NULL){

            strcpy(buf, pch);
            s = sb_stemmer_stem(stemmer, buf, strlen(buf)) ;
            if(!isInList(s)){
                for(int j=0; j<=wd->total; j++){
                    //If it is already in wordDic, increase count.
                    if(strcmp(s, wd->word[j]) == 0) {
                        (wd->count[j])++; break;
                    }
                    //If it is not in wordDic, add the word in dictionary.
                    if(j==wd->total){
                        strcpy(wd->word[wd->total], s);
                        wd->count[wd->total]++;
                        wd->total++;
                        break;
                    }
                }
            }
            

        pch=strtok(NULL, " ");
       
    }
    //Remove redundant words
    //Remove infrequent words

}


void printDic(struct wordDic *wd){

    for(int i=0; i<wd->total; i++){
        printf("{Word : %s  Count : %d}\n", wd->word[i], wd->count[i]);
    }

}

void printSwlist(){
    for(int i=0; i<MAX; i++){
        printf("%s", swlist[i]);
    }
}