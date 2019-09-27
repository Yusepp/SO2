#include <stdio.h>
#include <stdlib.h>

#define MAXCHAR 100

int main(void)
{
  FILE *fp;
  char word[MAXCHAR];

  fp = fopen("words", "r");
  if (!fp) {
    printf("Could not open file\n");
    exit(1);
  }

  while (fgets(word, MAXCHAR, fp))
    printf("%s", word);

  fclose(fp);

  return  0;
}


