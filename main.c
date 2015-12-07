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
#define COLOG_FIRST_REPLACE_CALL 1
#define COLOG_LAST_REPLACE_CALL 2
#define COLOG_NOT_KEYWORD_REPLACE_CALL 3

char* replace_with_color(const char* pattern, char* source, char* color, int all_line, int call_identifier)
{
  char* replaced;
  char* concated;
  int i;
  static int already_replaced = 0;
  static char* last_matched_color = KNRM;

  if (call_identifier == COLOG_FIRST_REPLACE_CALL && already_replaced) {
    already_replaced = 0;
  }
  if (!all_line || !already_replaced) {
    colog_matches_t matches = match(pattern, source);

    if (all_line && (matches.length || call_identifier == COLOG_LAST_REPLACE_CALL)) {
      if (!matches.length) {
        color = last_matched_color;
      }
      concated = xalloc(sizeof(*concated) * strlen(source) + 15);
      sprintf(concated, "%s%s%s", color, source, RESET);
      free(source);
      source = concated;
      if (matches.length && call_identifier != COLOG_NOT_KEYWORD_REPLACE_CALL) {
        last_matched_color = color;
      }
    }
    for (i = 0 ; i < matches.length ; i++) {
      if (!all_line) {
        concated = xalloc(sizeof(*concated) * strlen(source) + 15);
        sprintf(concated, "%s%s%s", color, matches.list[i], RESET);
        replaced = str_replace(source, matches.list[i], concated);
        free(concated);
        free(source);
        source = replaced;
      }
      free(matches.list[i]);
    }
    free(matches.list);
  }

  return source;
}

void colorize(FILE* file, int all_line)
{
  char *buffer = xalloc(BUFFERSIZE);

  while(fgets(buffer, BUFFERSIZE , file)) {
    buffer = replace_with_color("([[:lower:]]+\\.INFO)", buffer, KBLU, all_line, COLOG_FIRST_REPLACE_CALL);
    buffer = replace_with_color("([[:lower:]]+\\.DEBUG)", buffer, KCYN, all_line, 0);
    buffer = replace_with_color("([[:lower:]]+\\.ERROR)", buffer, KRED, all_line, 0);
    buffer = replace_with_color("([[:lower:]]+\\.CRITICAL)", buffer, KRED, all_line, 0);
    buffer = replace_with_color("([[:lower:]]+\\.WARNING)", buffer, KYEL, all_line, COLOG_LAST_REPLACE_CALL);
    buffer = replace_with_color("(\\[[[:digit:]]{4}-[[:digit:]]{2}-[[:digit:]]{2} [[:digit:]]{2}:[[:digit:]]{2}:[[:digit:]]{2}\\])", buffer, KGRN, all_line, COLOG_NOT_KEYWORD_REPLACE_CALL);
    buffer = replace_with_color("(\\\"[^\"]+\\\")", buffer, LGRA, all_line, COLOG_NOT_KEYWORD_REPLACE_CALL);
    buffer = replace_with_color("('[^']+')", buffer, LGRA, all_line, COLOG_NOT_KEYWORD_REPLACE_CALL);

    write(STDOUT, buffer, strlen(buffer));
    free(buffer);
    buffer = xalloc(BUFFERSIZE);
  }
  free(buffer);
}

int main(int argc, char **argv)
{
  if (has_option('h', argc, argv)) {
    printf("Usage: colog [-h] [-l] [-w] [FILE]\n\n-h: Displays this message\n-l: Colors the whole line instead of keywords\n-w: Waits at the end of the file for new entries\nFILE: The name of your file. If not provided, colog will listen to the standard input.\n");
    exit(0);
  }

  char* filename = get_argument(argc, argv);
  FILE* file;
  int all_line = has_option('l', argc, argv) ? 1 : 0;

  if (filename == NULL) {
    file = stdin;
  } else {
    file = fopen(filename, "r");
  }

  if (has_option('w', argc, argv)) {
    fseek(file, 0, SEEK_END);
    while (1) {
      colorize(file, all_line);
      usleep(500);
    }
  } else {
    colorize(file, all_line);
  }

  if (filename != NULL) {
    fclose(file);
  }

  return 0;
}
