#ifndef __COLOG_YML__
#define __COLOG_YML__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

typedef struct colog_yaml_tree_s {
  char*                      key;
  struct colog_yaml_tree_s** children;
  int                        nb_children;
  char*                      value;
  int                        level;
  struct colog_yaml_tree_s*  parent;
} colog_yaml_tree_t;

/**
 * Parses the given configuration file
 */
colog_yaml_tree_t* parse_configuration(const char* filename);

/**
 * Builds a configuration tree node
 */
colog_yaml_tree_t* treat_configuration_line(char* line, colog_yaml_tree_t* parent);

/**
 * Returns the amount of beginning spaces
 */
int nb_spaces_at_beginning(const char* line);

/**
 * Initializes a configuration node
 */
void initialize_yaml_node(colog_yaml_tree_t* node);

/**
 * Given the line, fills the node with the accurate data
 */
void extract_yaml_data_from_line(char* line, colog_yaml_tree_t* node);

/**
 * Retrieves in tree's children the one that has the given key
 */
colog_yaml_tree_t* find_configuration_node(const char* key, colog_yaml_tree_t* tree);

#endif /* __COLOG_YML__ */
