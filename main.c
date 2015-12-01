#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "colors.h"
#include "utils.h"

#define STDIN 0
#define STDOUT 1
#define BUFFERSIZE 1024

char* colorize(const char* pattern, char* source, const char* color)
{
  char* replaced;
  char* concated;
  int i;
  colog_matches_t matches = match(pattern, source);

  for (i = 0 ; i < matches.length ; i++) {
    concated = xalloc(sizeof(*replaced) * strlen(source) + 15);
    sprintf(concated, "%s%s%s", color, matches.list[i], RESET);
    replaced = str_replace(source, matches.list[i], concated);
    free(concated);
    free(source);
    free(matches.list[i]);
    source = replaced;
  }
  free(matches.list);

  return source;
}

int main(int argc, char **argv)
{
  char *buffer = xalloc(BUFFERSIZE);

  while(fgets(buffer, BUFFERSIZE , stdin)) {
    buffer = colorize("(\\[[[:digit:]]{4}-[[:digit:]]{2}-[[:digit:]]{2} [[:digit:]]{2}:[[:digit:]]{2}:[[:digit:]]{2}\\])", buffer, KGRN);
    buffer = colorize("([[:lower:]]+\\.INFO)", buffer, KBLU);
    buffer = colorize("([[:lower:]]+\\.DEBUG)", buffer, KCYN);
    buffer = colorize("([[:lower:]]+\\.ERROR)", buffer, KRED);
    buffer = colorize("([[:lower:]]+\\.CRITICAL)", buffer, KRED);
    buffer = colorize("([[:lower:]]+\\.WARNING)", buffer, KYEL);
    buffer = colorize("(\\\"[^\"]+\\\")", buffer, KMAG);

    write(STDOUT, buffer, strlen(buffer));
    free(buffer);
    buffer = xalloc(BUFFERSIZE);
  }

  return 0;
}
