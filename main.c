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

char* replace_with_color(const char* pattern, char* source, const char* color)
{
  char* replaced;
  char* concated;
  int i;
  colog_matches_t matches = match(pattern, source);

  for (i = 0 ; i < matches.length ; i++) {
    concated = xalloc(sizeof(*concated) * strlen(source) + 15);
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

void colorize(FILE* file)
{
  char *buffer = xalloc(BUFFERSIZE);

  while(fgets(buffer, BUFFERSIZE , file)) {
    buffer = replace_with_color("(\\[[[:digit:]]{4}-[[:digit:]]{2}-[[:digit:]]{2} [[:digit:]]{2}:[[:digit:]]{2}:[[:digit:]]{2}\\])", buffer, KGRN);
    buffer = replace_with_color("([[:lower:]]+\\.INFO)", buffer, KBLU);
    buffer = replace_with_color("([[:lower:]]+\\.DEBUG)", buffer, KCYN);
    buffer = replace_with_color("([[:lower:]]+\\.ERROR)", buffer, KRED);
    buffer = replace_with_color("([[:lower:]]+\\.CRITICAL)", buffer, KRED);
    buffer = replace_with_color("([[:lower:]]+\\.WARNING)", buffer, KYEL);
    buffer = replace_with_color("(\\\"[^\"]+\\\")", buffer, KMAG);

    write(STDOUT, buffer, strlen(buffer));
    free(buffer);
    buffer = xalloc(BUFFERSIZE);
  }
  free(buffer);
}

int main(int argc, char **argv)
{
  FILE* file;

  if (has_option('h', argc, argv)) {
    printf("Usage: colog [-h] [-w] [filename]\n\n-h: Displays this message\n-w: Waits at the end of the file for new entries\nfilename: The name of your file. If not provided, colog will listen to the standard input.\n");
    exit(0);
  }

  char* filename = get_argument(argc, argv);

  if (filename == NULL) {
    file = stdin;
  } else {
    file = fopen(filename, "r");
  }

  if (has_option('w', argc, argv)) {
    fseek(file, 0, SEEK_END);
    while (1) {
      colorize(file);
      usleep(500);
    }
  } else {
    colorize(file);
  }

  if (filename != NULL) {
    fclose(file);
  }

  return 0;
}
