/*


Read Tools: provide functions to create pointers from files.

We create separate them from main for better code comprehesion.



*/

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>  
#include <stdio.h>
#include <stdarg.h>

#define MAXCHAR 100
#define DATABASE "./base_dades/"
#define DICTIONARY "./diccionari/words"

//Header for import functions

//FOR DICTIONARY
int countDicWords(char *file);
char **getDictionary(char * file,int num_words);
//FOR FILES
int countWords_inLine(char *line);
int process_line(char **words,char *line,int index);
char **process_file(char *file,int *file_words);
//FOR LIST
int countItems(char *file);
char **getListItems(char * file,int num_items);
//DELETE ARRAY OF POINTERS
void deletepointers(char **pointer,int num_words);
