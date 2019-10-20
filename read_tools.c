
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>  
#include <stdio.h>
#include <stdarg.h>
#include "read_tools.h"

#define MAXCHAR 100
#define DATABASE "./base_dades/"
#define DICTIONARY "./diccionari/words"

/*


Read Tools: provide functions to create pointers from files.

We create separate them from main for better code comprehesion.


*/



//Function Declaration



/*
    FUNCTIONS FOR DICTIONARY
*/

//Count how many words in dictionary.
int countDicWords(char *file){
	FILE *fp;
	char *word;
	int counter;
    //open file
	fp = fopen(file, "r");
	if(fp == NULL){//check for error
		printf("Error opening: %s\n",file);
		exit(1);
	}
	//Counting how many words we gonna read.
	counter = 0;
	word = malloc(sizeof(char) * MAXCHAR);//maximum 100 chars
    if(word == NULL){//check for error
		printf("Cannot allocate memory for dicWord\n");
		exit(2);
	}
	while (fgets(word, MAXCHAR, fp)){//every line contain a word so let's count lines
		counter++;
	}
    //we free memory
	free(word);
	fclose(fp);

	return counter-1;//reads 1 empty lane.

}
//Saves every word creating an array of char *.
char **getDictionary(char * file,int num_words){
	//Initialize variables
	char ** getDicWords;
	char *tmp;
	FILE *fp;
	int i;

	fp = fopen(file,"r");//read file
	//checking for errors at reading
	if(fp == NULL){//check for error
		printf("Error opening: %s\n",file);
		exit(1);
	}
    //for words
	tmp = malloc(sizeof(char) * MAXCHAR);
	//Now we can malloc our main pointer
	getDicWords = malloc(num_words*sizeof(tmp));
	if(getDicWords == NULL || tmp == NULL){//check for errors
		printf("Cannot allocate memory for dictionary\n");
		exit(2);
	}
	i = 0;
	//we now must start reading
	while (fgets(tmp, MAXCHAR, fp)){
        tmp[strlen(tmp)-1] = '\0';//adding the 0 to the last position for strlen
        getDicWords[i] = strdup(tmp);//create a copy of the pointer
        i++;
	}
    //free memory
    free(tmp);
	fclose(fp);
	//return pointer
	return getDicWords;
}


/*
    FUNCTIONS FOR FILES
*/

//Count how many words are in the current line
//I tried to remove char *paraula but this freezes computer for no reason.
int countWords_inLine(char *line){

    int i, j, is_word, len_line,words_found;
    i = 0;
    char *paraula;
    paraula = malloc(MAXCHAR * sizeof(char));
    len_line = strlen(line);

    /* Search for the beginning of a candidate word */

    while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    /* This is the main loop that extracts all the words */

    while (i < len_line)
    {
        j = 0;
        is_word = 1;

        /* Extract the candidate word including digits if they are present */

        do {
            if(isalpha(line[i]) || isdigit(line[i]) || ispunct(line[i])){
                paraula[j] = line[i];
            }
            else{ 
                is_word = 0;
            }
            j++; i++;


            /* Check if we arrive to an end of word: space or punctuation character */

        } while ((i < len_line) && (!isspace(line[i])) && (!ispunct(line[i]) || (line[i] == '\'') || (line[i] == '-')));

        /* If word we increase count */

        if (is_word) {
            words_found++;
        }

        /* Search for the beginning of a candidate word */

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    }
    //free memory
    free(paraula);
    return words_found;//return words in this line.
}

//Process for lines, find every word in line and save into main pointer.
int process_line(char **words,char *line,int index){

    int i, j, is_word, len_line,words_found;
    char *paraula;
    i = 0;
    paraula = malloc(MAXCHAR*sizeof(char));
    //check for error.
    if(paraula == NULL){
        printf("Cannot allocate memory for fileWord\n");
		exit(2);
    }

    len_line = strlen(line);

    /* Search for the beginning of a candidate word */

    while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    /* This is the main loop that extracts all the words */

    while (i < len_line)
    {
        j = 0;
        is_word = 1;

        /* Extract the candidate word including digits if they are present */

        do {
            if(isalpha(line[i]) || isdigit(line[i]) || ispunct(line[i])){
                paraula[j] = line[i];
            }
            else{ 
                is_word = 0;
            }
            j++; i++;

            /* Check if we arrive to an end of word: space or punctuation character */

        } while ((i < len_line) && (!isspace(line[i])) && (!ispunct(line[i]) || (line[i] == '\'') || (line[i] == '-')));

        /* If word insert in list */

        if (is_word) {
            /* Put a '\0' (end-of-word) at the end of the string*/
            paraula[j] = '\0';
            words[index] = strdup(paraula);//copy word to main pointer
            index++;//increase main index
        }

        /* Search for the beginning of a candidate word */

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    } 
    //free memory
    free(paraula);
    //return the index of the words so we start where we finished last time.
    return index;
}

//Process an entire file processing every line
char **process_file(char *file,int *file_words){
    FILE *fp;
    char **words;
    char *tmp;
    int index = 0,i;
    int total_words = 0;
    
    fp = fopen(file, "r");
    if (!fp) {//checking errors
        printf("Could not open: %s\n",file);
        exit(1);
    }
    
    tmp = malloc(MAXCHAR*sizeof(char));//saving lines
    if(tmp == NULL){//checking errors
        printf("Cannot allocate memory for fileLine\n");
		exit(2);
    }

    //Start reading lines
    while (fgets(tmp, MAXCHAR, fp)){
        total_words += countWords_inLine(tmp);//count how many words are in file.
    }
        

    words = malloc(total_words*sizeof(tmp));//main pointer
    if(tmp == NULL){//checking errors
        printf("Cannot allocate memory for mainFile pointer\n");
        exit(3);
    }

    //restart variables
    i = 0;
    rewind(fp);

    //reading every line again but this time saving words
    while (fgets(tmp, MAXCHAR, fp)){
        index = process_line(words,tmp,index);//save words in every line.
    }

    *file_words = total_words;//save total words in pointer
    //free memory
    free(tmp);
    fclose(fp);
    //return main pointer with file words
    return words;
}

/*
    FUNCTIONS FOR LISTS
*/

//Count how many items in list
int countItems(char *file){
	FILE *fp;
	char *item;
	int counter;
    //open file
	fp = fopen(file, "r");
	if(fp == NULL){//check for error
		printf("Error opening: %s\n",file);
		exit(1);
	}
	//Counting how many words we gonna read.
	counter = 0;
	item = malloc(sizeof(char) * MAXCHAR);//maximum 100 chars
    if(item == NULL){//check for error
		printf("Cannot allocate memory for item\n");
		exit(2);
	}
	while (fgets(item, MAXCHAR, fp)){//every line contain a word so let's count lines
        if(strlen(item) > 4){//its not initial number
            counter++;
        }
	}
    //we free memory
	free(item);
	fclose(fp);

	return counter;//reads 1 empty lane.

}

//Saves every word creating an array of char *.
char **getListItems(char * file,int num_items){
	//Initialize variables
	char ** items;
	char *item;
	FILE *fp;
	int i;

	fp = fopen(file,"r");//read list
	//checking for errors at reading
	if(fp == NULL){//check for error
		printf("Error opening: %s\n",file);
		exit(1);
	}
    //for item
	item = malloc(sizeof(char) * MAXCHAR);
	//Now we can malloc our main pointer
	items = malloc(num_items*sizeof(item));
	if(items == NULL || item == NULL){//check for errors
		printf("Cannot allocate memory for items list\n");
		exit(2);
	}
	i = 0;
	//we now must start reading
	while (fgets(item, MAXCHAR, fp)){
        if(strlen(item) > 4){//its not initial number
            item[strlen(item)-1] = '\0';//adding the 0 to the last position for strlen
            items[i] = strdup(item);//create a copy of the pointer
            i++;
        }
	}
    //free memory
    free(item);
	fclose(fp);
	//return pointer
	return items;
}


//DELETE ARRAY OF POINTERS
void deletepointers(char **pointer,int num_words){
	int i;
	for(i = 0; i< num_words; i++){
		free(pointer[i]);
	}
	free(pointer);
}