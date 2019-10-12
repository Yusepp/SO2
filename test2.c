#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define MAXVALUE 10
#define MAXCHAR 100

int countFileWords(char *file);
char **getWords(char * file,int num_words);
void showdict(char **dict,int num_words);
void deletedict(char **dict,int num_words);


int main(int argc, char const *argv[])
{
	char ** dict;
	int num_words = 0;
	num_words = countFileWords("./diccionari/words");
	dict = getWords("./diccionari/words",num_words);
	showdict(dict,num_words);
	deletedict(dict,num_words);
	
	return 0;
}


int countFileWords(char *file){
	FILE *fp;
	char *word;
	int counter;
	fp = fopen(file, "r");
	if(fp == NULL){
		printf("Error opening file\n");
		exit(1);
	}

	//Counting how many words we gonna read.
	counter = 0;
	word = malloc(sizeof(char) * MAXCHAR);
	while (fgets(word, MAXCHAR, fp)){
		counter++;
	}
	free(word);
	fclose(fp);
	return counter;

}

char **getWords(char * file,int num_words){
	//Initialize variables
	char ** getDicWords;
	char *tmp;
	FILE *fp;
	char *word;
	int i;

	fp = fopen(file,"r");//read file
	//checking for errors at reading
	if(fp == NULL){
		printf("Error opening file\n");
		exit(1);
	}
	tmp = malloc(sizeof(char) * MAXCHAR);
	//Now we can malloc our main pointer
	getDicWords = malloc(num_words*sizeof(*getDicWords));
	//and in the subpointers
	for(i = 0; i < num_words; i++){
		getDicWords[i] = malloc((MAXCHAR + 1) * sizeof(**getDicWords));
	}
	i = 0;
	//we now must start reading
	while (fgets(tmp, MAXCHAR, fp)){
		//printf("%s\n",tmp);
		getDicWords[i] = strdup(tmp);
		i++;
	}
	fclose(fp);
	//return pointer
	return getDicWords;



}

void showdict(char **dict,int num_words){
	int i;
	for(i = 0; i< num_words; i++){
		printf("%s\n",dict[i]);
	}
}

void deletedict(char **dict,int num_words){
	int i;
	for(i = 0; i< num_words; i++){
		free(dict[i]);
	}
	free(dict);
}
