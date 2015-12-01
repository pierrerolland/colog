#ifndef __COLOG_UTILS__
#define __COLOG_UTILS__

typedef struct colog_matches_s {
  int    length;
  char** list;
} colog_matches_t;

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

#endif /* __COLOG_UTILS__ */
