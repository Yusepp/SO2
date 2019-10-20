#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  

#define MAXCHAR 100

/**
 * 
 * Given a line, extract the words it contains and print them to screen
 * 
 */ 

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
		if(strlen(word) > 4){
		counter++;
		}
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
	int i;

	fp = fopen(file,"r");//read file
	//checking for errors at reading
	if(fp == NULL){
		printf("Error opening file\n");
		exit(1);
	}
	tmp = malloc(sizeof(char) * MAXCHAR);
	//Now we can malloc our main pointer
	getDicWords = malloc(num_words*sizeof(tmp));
	//and in the subpointers
	i = 0;
	//we now must start reading
	while (fgets(tmp, MAXCHAR, fp)){
		//printf("%s\n",tmp);
    if(strlen(tmp) > 4){
      tmp[strlen(tmp)-1] = '\0';
		  getDicWords[i] = strdup(tmp);
		  i++;
    }
   
	}
  free(tmp);
	fclose(fp);
	//return pointer
	return getDicWords;



}

void showPointer(char **pointer,int num_words){
	int i;
	for(i = 0; i< num_words; i++){
		printf("%s\n",pointer[i]);
	}
}

void deletepointers(char **pointer,int num_words){
	int i;
	for(i = 0; i< num_words; i++){
		free(pointer[i]);
	}
	free(pointer);
}

char * createPath(char *subpath){
	char *start = "./base_dades/";
	char *tmp = malloc(sizeof(char)*(strlen(start)+strlen(subpath)+1));
	strcpy(tmp,start);
	strcat(tmp,subpath);
	return tmp;
}

int main(int argc, char **argv)
{
    int files = countFileWords("base_dades/llista_10.cfg");
    char **files_as_char = getWords("base_dades/llista_10.cfg",files);
    showPointer(files_as_char,files);
	
	FILE *fp = fopen(createPath(files_as_char[0]),"r");
	char *tmp = malloc(sizeof(char)*MAXCHAR);
	for(int i = 0; i < 100; i++){
		fgets(tmp, MAXCHAR, fp);
		printf("%s\n",tmp);
	}
    deletepointers(files_as_char,files);

    
    return 0;
}
