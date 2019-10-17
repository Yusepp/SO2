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

void showFile(char **words,int total_words){
    for(int i = 0; i < total_words; i++){
        printf("%s\n",words[i]);
    }
}

int count_line_words(char *line)
{
    int i, j, is_word, len_line,words_found;
    char *paraula;
    i = 0;
    paraula = malloc(MAXCHAR*sizeof(*paraula));
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
            paraula[j] = 0;
            words_found++;
        }

        /* Search for the beginning of a candidate word */

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    } /* while (i < len_line) */
    free(paraula);
    return words_found;
}

int process_line(char **words,char *line,int index)
{
    int i, j, is_word, len_line,words_found;
    char *paraula;
    i = 0;
    paraula = malloc(MAXCHAR*sizeof(*paraula));
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
            paraula[j] = 0;
            words[index] = strdup(paraula);
            //printf("%s\n",words[index]);
            index++;
        }

        /* Search for the beginning of a candidate word */

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    } /* while (i < len_line) */
    free(paraula);
    return index;
}

char **process_file(char *file){
    FILE *fp;
    char **words;
    char *tmp;
    int index = 0,i;
    int total_words = 0;
    
    fp = fopen(file, "r");
    if (!fp) {
        printf("Could not open file\n");
        exit(1);
    }
    
    tmp = malloc(MAXCHAR*sizeof(*tmp));

    while (fgets(tmp, MAXCHAR, fp))
        total_words += count_line_words(tmp);

    words = malloc(total_words*sizeof(*words));
    //and in the subpointers
    for(i = 0; i < total_words; i++){
        words[i] = malloc((MAXCHAR + 1) * sizeof(**words));
    }

    i = 0;
    rewind(fp);

    while (fgets(tmp, MAXCHAR, fp))
        index = process_line(words,tmp,index);
    
    showFile(words,total_words);
    fclose(fp);
    return words;
}

void change(int * a){
    int b = 5;
    b++;
    *a = b;
}

int main(int argc, char **argv)
{
    //char ** file;
    //file = process_file("/home/yusepp/Documentos/SO2/P2/extraccio-paraules/file");
    int *a = NULL;
    printf("%d\n",*a);
    change(a);
    printf("%d\n",*a);
    free(a);
    return 0;
}
