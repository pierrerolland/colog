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

  for (i = argc - 1 ; i > 1 ; i--) {
    if (argv[i][0] != '-' && !(argv[i - 1][0] == '-' && (argv[i - 1][strlen(argv[i - 1]) - 1] == 'c' || argv[i - 1][strlen(argv[i - 1]) - 1] == 't'))) {
      return argv[i];
    }
  }

  return NULL;
}

int has_option(char option, int argc, char** argv)
{
  int i, j;

  for (i = 1 ; i < argc ; i++) {
    if (argv[i][0] == '-') {
      for (j = 1 ; j < strlen(argv[i]) ; j++) {
        if (argv[i][j] == option) {
          return 1;
        }
      }
    }
  }

  return 0;
}

char* get_option(char option, int argc, char** argv)
{
  int i;

  for (i = 1 ; i < argc ; i++) {
    if (argv[i][0] == '-' && argv[i][strlen(argv[i]) - 1] == option && (i + 1) < argc) {
      return argv[i + 1];
    }
  }

  return NULL;
}

char* trim(char *str, char to_trim)
{
  char *end;

  while (*str == to_trim) {
      str++;
  }

  if (*str == 0) {
      return str;
  }
  end = str + strlen(str) - 1;

  while (end > str && *end == to_trim) {
      end--;
  }
  *(end+1) = 0;

  return str;
}

void free_matches(colog_matches_t matches)
{
  int i;

  for (i = 0 ; i < matches.length ; i++) {
    free(matches.list[i]);
  }
  free(matches.list);
}

int strpos(char c, const char* str)
{
  int i, opened_quotes = 0;

  for (i = strlen(str) - 1 ; i > 0 ; i--) {
    if (str[i] == '"') {
      opened_quotes = (opened_quotes == 1 ? 0 : 1);
    }
    if (str[i] == c && opened_quotes == 0) {
      return i;
    }
  }

  return -1;
}

colog_explode_t explode(char delimiter, char* str)
{
  colog_explode_t exploded;
  int pos, end;

  exploded.list = NULL;
  exploded.length = 0;
  do {
    pos = strpos(delimiter, str);
    end = (pos == -1 ? strlen(str) - 2 : pos - 1);
    exploded.list = xrealloc(exploded.list, (exploded.length + 1) * sizeof(char *));
    exploded.list[exploded.length] = xalloc((end + 2) * sizeof(char));
    strncpy(exploded.list[exploded.length], str, end + 1);
    exploded.list[exploded.length][end + 1] = '\0';
    exploded.length++;
    str += end + 2;
  } while (pos != -1);

  return exploded;
}

void free_exploded(colog_explode_t exploded)
{
  int i;

  for (i = 0 ; i < exploded.length ; i++) {
    free(exploded.list[i]);
  }
  free(exploded.list);
}
