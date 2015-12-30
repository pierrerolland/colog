#ifndef __COLOG_UTILS__
#define __COLOG_UTILS__

typedef struct colog_matches_s {
  int    length;
  char** list;
} colog_matches_t;

typedef struct colog_explode_s {
  int    length;
  char** list;
} colog_explode_t;

/**
 * Secure malloc
 */
void* xalloc(size_t size);

/**
 * Secure realloc
 */
void* xrealloc(void *orig, size_t size);

/**
 * Replaces all occurences of "rep" with "with" in "orig"
 */
char *str_replace(char *orig, char *rep, char *with);

/**
 * Returns 1 if "str" was found in "array", 0 otherwise
 */
int  in_array(const char *str, char **array, int length);

/**
 * Returns all regex matches of "pattern" in "source"
 */
colog_matches_t match(const char *pattern, const char *source);

/**
 * Gets the first non-option argument
 */
char* get_argument(int argc, char** argv);

/**
 * Checks if has option
 */
int has_option(char option, int argc, char** argv);

/**
 * Retrieves the option, NULL if not found
 */
char* get_option(char option, int argc, char** argv);

/**
 * Trims leading and ending to_trim of a string
 */
char *trim(char *str, char to_trim);

/**
 * Frees the list of matches
 */
void free_matches(colog_matches_t matches);

/**
 * Finds the position of c in str
 */
int strpos(char c, const char* str);

/**
 * Explodes a string into an array of strings, given a delimiter
 */
colog_explode_t explode(char delimiter, char* str);

/**
 * Frees the exploded array
 */
void free_exploded(colog_explode_t exploded);

#endif /* __COLOG_UTILS__ */
