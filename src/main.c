#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "colors.h"
#include "utils.h"
#include "yml.h"

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

void colorize(FILE* file, int all_line, colog_yaml_tree_t* configuration)
{
  char *buffer = xalloc(BUFFERSIZE);
  int i;
  int call;
  colog_yaml_tree_t* keywords = find_configuration_node("keywords", configuration);
  colog_yaml_tree_t* regular = find_configuration_node("regular", configuration);

  while(fgets(buffer, BUFFERSIZE , file)) {
    for (i = 0 ; i < keywords->nb_children ; i++) {
      if (i == 0) {
        call = COLOG_FIRST_REPLACE_CALL;
      } else {
        call = (i == keywords->nb_children - 1 ? COLOG_LAST_REPLACE_CALL : 0);
      }
      buffer = replace_with_color(keywords->children[i]->key, buffer, convert_color(keywords->children[i]->value), all_line, call);
    }
    for (i = 0 ; i < regular->nb_children ; i++) {
      buffer = replace_with_color(regular->children[i]->key, buffer, convert_color(regular->children[i]->value), all_line, COLOG_NOT_KEYWORD_REPLACE_CALL);
    }
    write(STDOUT, buffer, strlen(buffer));
    free(buffer);
    buffer = xalloc(BUFFERSIZE);
  }
  free(buffer);
}

int main(int argc, char **argv)
{
  if (has_option('h', argc, argv)) {
    printf("Usage: colog -d DEFINITION [-h] [-l] [-w] [-c CONFIG_FILE] [FILE]\n\n-c: Location of the configuration file. Default: /etc/colog/colog.yml\n-d: Specifies the log definition, available in the configuration file\n-h: Displays this message\n-l: Colors the whole line instead of keywords\n-w: Waits at the end of the file for new entries\nFILE: The name of your file. If not provided, colog will listen to the standard input.\n");
    exit(0);
  }

  char* log_definition = get_option('d', argc, argv);

  if (log_definition == NULL) {
    printf("You must specify a log definition.\ncolog -h to display help\n");
    exit(-1);
  }

  char* conf_file = get_option('c', argc, argv);

  if (conf_file == NULL) {
    conf_file = "/etc/colog/colog.yml";
  }

  char* filename = get_argument(argc, argv);
  FILE* file;
  int all_line = has_option('l', argc, argv);
  colog_yaml_tree_t* configuration = find_configuration_node(log_definition, parse_configuration(conf_file));

  if (filename == NULL) {
    file = stdin;
  } else {
    file = fopen(filename, "r");
    if (!file) {
      printf("Couldn't open file\n");
      exit(-1);
    }
  }

  if (has_option('w', argc, argv)) {
    fseek(file, 0, SEEK_END);
    while (1) {
      colorize(file, all_line, configuration);
      usleep(500);
    }
  } else {
    colorize(file, all_line, configuration);
  }

  if (filename != NULL) {
    fclose(file);
  }

  return 0;
}
