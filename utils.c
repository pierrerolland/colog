#include <stdlib.h>
#include <string.h>

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

  tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

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
