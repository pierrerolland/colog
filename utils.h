#ifndef __COLOG_UTILS__
#define __COLOG_UTILS__

/**
 * Replaces all occurences of "rep" with "with" in "orig"
 */
char *str_replace(char* orig, char* rep, char* with);

/**
 * Returns 1 if "str" was found in "array", 0 otherwise
 */
int  in_array(const char* str, char** array, int length);

#endif /* __COLOG_UTILS__ */
