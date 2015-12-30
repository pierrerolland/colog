#ifndef __COLOG_COLORS__
#define __COLOG_COLORS__

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define LGRA  "\e[37m"
#define DGRA  "\e[90m"
#define RESET "\033[0m"

char* convert_color(char* color);

#endif /* __COLOG_COLORS__ */
