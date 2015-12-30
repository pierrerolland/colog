#include "yml.h"

colog_yaml_tree_t* parse_configuration(const char* filename)
{
  colog_yaml_tree_t* node = xalloc(sizeof(colog_yaml_tree_t));
  FILE* file = fopen(filename, "r");
  char line[256];
  int line_index = 1;
  if (file == NULL) {
    printf("[configuration] Unable to read configuration file\n");
    exit(-1);
  }
  initialize_yaml_node(node);
  node->level = 0;
  while (fgets(line, 256, file)) {
    if (strlen(line) == 256) {
      printf("[configuration] Line %d too long. Aborting.\n", line_index);
      exit(-1);
    }
    if (line[0] == '#') {
      continue;
    }
    node = treat_configuration_line(line, node);
    line_index++;
  }

  fclose(file);

  while (node->parent != NULL) {
    node = node->parent;
  }

  return node;
}

colog_yaml_tree_t* treat_configuration_line(char* line, colog_yaml_tree_t* parent)
{
  colog_yaml_tree_t* node = xalloc(sizeof(colog_yaml_tree_t));
  int level = (nb_spaces_at_beginning(line) / 4) + 1;

  initialize_yaml_node(node);
  while (level <= parent->level && parent->parent != NULL) {
    parent = parent->parent;
  }

  node->level = level;
  node->parent = parent;
  parent->children = xrealloc(parent->children, (parent->nb_children + 1) * sizeof(colog_yaml_tree_t*));
  parent->children[parent->nb_children] = node;
  parent->nb_children++;

  extract_yaml_data_from_line(line, node);

  return node;
}

colog_yaml_tree_t* find_configuration_node(const char* key, colog_yaml_tree_t* tree)
{
  int i;

  for (i = 0 ; i < tree->nb_children ; i++) {
    if (!strcmp(tree->children[i]->key, key)) {
      return tree->children[i];
    }
  }

  printf("[configuration] Couldn't find child %s\n", key);
  exit(-1);
}

void extract_yaml_data_from_line(char* line, colog_yaml_tree_t* node)
{
  line = trim(line, ' ');
  colog_matches_t matches;
  colog_explode_t exploded;

  matches = match("(\\\".+\\\"):[ ]*(\\\".+\\\")", line);
  if (matches.length == 1) {
    exploded = explode(':', line);
    if (exploded.length == 2) {
      node->key = xalloc((strlen(exploded.list[0]) - 1) * sizeof(char));
      node->value = xalloc((strlen(exploded.list[1]) - 1) * sizeof(char));
      strncpy(node->key, &(exploded.list[0][1]), strlen(exploded.list[0]) - 2);
      strncpy(node->value, &(exploded.list[1][1]), strlen(exploded.list[1]) - 2);
      node->key[strlen(exploded.list[0]) - 2] = '\0';
      node->value[strlen(exploded.list[1]) - 2] = '\0';
      free_exploded(exploded);
    } else {
      printf("[configuration] syntax error\n");
      exit(-1);
    }
  } else {
    node->key = xalloc((strlen(line) - 1) * sizeof(char));
    strncpy(node->key, line, strlen(line) - 2);
    node->key[strlen(line) - 2] = '\0';
  }

  free_matches(matches);
}

int nb_spaces_at_beginning(const char* line)
{
  int i = 0;

  while (line[i] == ' ') {
    i++;
  }

  return i;
}

void initialize_yaml_node(colog_yaml_tree_t* node)
{
  node->key = NULL;
  node->nb_children = 0;
  node->children = NULL;
  node->value = NULL;
  node->parent = NULL;
  node->level = -1;
}
