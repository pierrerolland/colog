#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void* xalloc(size_t size)
{
  void* ptr = malloc(size);

  if (ptr == NULL) {
    printf("Error. No memory.\n");
    exit(-1);
  }

  return ptr;
}

void* xrealloc(void *orig, size_t size)
{
  void* ptr = realloc(orig, size);

  if (ptr == NULL) {
    printf("Error. No memory.\n");
    exit(-1);
  }

  return ptr;
}

char *str_replace(char *orig, char *rep, char *with)
{
  char *result;
  char *ins;
  char *tmp;
  int len_rep;
  int len_with;
  int len_front;
  int count;

  if (!orig) {
    return NULL;
  }
  if (!rep) {
    rep = "";
  }
  len_rep = strlen(rep);
  if (!with) {
    with = "";
  }
  len_with = strlen(with);
  ins = orig;
  for (count = 0; tmp = strstr(ins, rep); ++count) {
    ins = tmp + len_rep;
  }

  tmp = result = xalloc(strlen(orig) + (len_with - len_rep) * count + 1);

  if (!result) {
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

int in_array(const char *str, char **array, int length)
{
  int i;

  for (i = 0 ; i < length ; i++) {
    if (!strcmp(str, array[i])) {
      return 1;
    }
  }

  return 0;
}

char* execute_regex(const char *pattern, const char *source)
{
  regex_t preg;
  regmatch_t *pmatch = NULL;
  char *extract = NULL;
  int status;
  size_t size;

  if (regcomp(&preg, pattern, REG_EXTENDED) == 0) {
    pmatch = xalloc(sizeof (*pmatch));
    if (pmatch) {
      status = regexec(&preg, source, 2, pmatch, 0);
      regfree(&preg);
      if (status == 0) {
        size = pmatch[0].rm_eo - pmatch[0].rm_so;
        if (size) {
          extract = xalloc(sizeof (*extract) * (size + 1));
          strncpy(extract, &source[pmatch[0].rm_so], size);
          extract[size] = '\0';
        }
      }
      free(pmatch);
    }
  }

  return extract;
}

colog_matches_t match(const char *pattern, const char *source)
{
  colog_matches_t matches;
  int analyzed_length;
  char* extract = NULL;
  char* analyzed = xalloc((strlen(source) + 1) * sizeof(char));
  char* tmp;
  int cont = 1;

  matches.list = NULL;
  matches.length = 0;
  strcpy(analyzed, source);
  while (cont) {
    extract = execute_regex(pattern, analyzed);
    if (extract) {
      if (!in_array(extract, matches.list, matches.length)) {
        matches.list = xrealloc(matches.list, (matches.length + 1) * sizeof(char *));
        matches.list[matches.length] = xalloc((strlen(extract) + 1) * sizeof(char));
        strcpy(matches.list[matches.length], extract);
        matches.length++;
        tmp = str_replace(analyzed, extract, "");
        free(analyzed);
        analyzed = tmp;
      }
      free(extract);
    } else {
      cont = 0;
    }
  }
  free(analyzed);

  return matches;
}

char* get_argument(int argc, char** argv)
{
  int i;

  for (i = 1 ; i < argc ; i++) {
    if (argv[i][0] != '-') {
      return argv[i];
    }
  }

  return NULL;
}

int has_option(char option, int argc, char** argv)
{
  int i;
  char full_option_name[2];

  sprintf(full_option_name, "-%c", option);
  for (i = 1 ; i < argc ; i++) {
    if (!strcmp(argv[i], full_option_name)) {
      return 1;
    }
  }

  return 0;
}
