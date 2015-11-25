#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "colors.h"

#define STDIN 0
#define BUFFERSIZE 1024

char *str_replace(char *orig, char *rep, char *with)
{
  char *result;
  char *ins;
  char *tmp;    
  int len_rep;  
  int len_with;
  int len_front;
  int count;

  if (!orig)
    {
      return NULL;
    }
  if (!rep)
    {
      rep = "";
    }
  len_rep = strlen(rep);
  if (!with)
    {
      with = "";
    }
  len_with = strlen(with);

  ins = orig;
  for (count = 0; tmp = strstr(ins, rep); ++count) {
    ins = tmp + len_rep;
  }

  tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

  if (!result)
    {
      return NULL;
    }

  while (count--) {
    ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep;
  }
  strcpy(tmp, orig);
  
  return result;
}

int in_array(const char* str, char** array, int length)
{
  int i;

  for (i = 0 ; i < length ; i++) {
    if (!strcmp(str, array[i])) {
      return 1;
    }
  }

  return 0;
}

char* colorize(const char* pattern, char* source, const char* color)
{
  int err;
  regex_t preg;
  int match;
  size_t nmatch = 0;
  regmatch_t *pmatch = NULL;
  char* extract = NULL;
  int start;
  int end;
  size_t size;
  char* replaced;
  char* concated;
  int i;
  char** extractedList = malloc(1);
  int nbExtracted = 0;
  char* sourceOrig = malloc((strlen(source) + 1) * sizeof(char));
      
  strcpy(sourceOrig, source);
  err = regcomp (&preg, pattern, REG_EXTENDED);
  if (err == 0)
    {
      nmatch = preg.re_nsub;
      pmatch = malloc (sizeof (*pmatch) * nmatch);
      if (pmatch)
	{
          match = regexec (&preg, source, nmatch, pmatch, 0);
	  regfree (&preg);
	  if (match == 0)
	    {
        for (i = 0; i < preg.re_nsub; i++) {
          start = pmatch[i].rm_so;
          end = pmatch[i].rm_eo;
          size = end - start;
          extract = malloc (sizeof (*extract) * (size + 1));
          if (extract)
            {
              strncpy(extract, &sourceOrig[start], size);
              extract[size] = '\0';
              if (!in_array(extract, extractedList, nbExtracted)) {
                concated = malloc(sizeof(*replaced) * strlen(source) + 15);
                sprintf(concated, "%s%s%s", color, extract, RESET);
                replaced = str_replace(source, extract, concated);
                extractedList = realloc(extractedList, sizeof(char*));
                extractedList[nbExtracted] = malloc((strlen(extract) + 1) * sizeof(char));
                strcpy(extractedList[nbExtracted], extract);
                nbExtracted++;
                free(concated); 
                free(extract);
                free(source);

                source = replaced;
              }
            }
        }

            }
         }
    }
  free(sourceOrig);

  return source;
}

int main(int argc, char **argv)
{
  char *buffer = malloc(BUFFERSIZE);

  while(fgets(buffer, BUFFERSIZE , stdin))
    {
      buffer = colorize("(\\[[[:digit:]]{4}-[[:digit:]]{2}-[[:digit:]]{2} [[:digit:]]{2}:[[:digit:]]{2}:[[:digit:]]{2}\\])", buffer, KGRN);
      buffer = colorize("([[:lower:]]+\\.INFO)", buffer, KBLU);
      buffer = colorize("([[:lower:]]+\\.DEBUG)", buffer, KCYN);
      buffer = colorize("([[:lower:]]+\\.ERROR)", buffer, KRED);
      buffer = colorize("([[:lower:]]+\\.CRITICAL)", buffer, KRED);
      buffer = colorize("([[:lower:]]+\\.WARNING)", buffer, KYEL);
      
      buffer = colorize("(\\\"[^\"]+\\\")", buffer, KMAG);
                  
      printf("%s", buffer);
      free(buffer);
      buffer = malloc(BUFFERSIZE);
    }
  
  return 0;
}
