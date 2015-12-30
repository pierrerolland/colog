#include "colors.h"

char* convert_color(char* color)
{
  if (!strcmp(color, "normal")) {
    return KNRM;
  }
  if (!strcmp(color, "red")) {
    return KRED;
  }
  if (!strcmp(color, "green")) {
    return KGRN;
  }
  if (!strcmp(color, "yellow")) {
    return KYEL;
  }
  if (!strcmp(color, "blue")) {
    return KBLU;
  }
  if (!strcmp(color, "magenta")) {
    return KMAG;
  }
  if (!strcmp(color, "cyan")) {
    return KCYN;
  }
  if (!strcmp(color, "white")) {
    return KWHT;
  }
  if (!strcmp(color, "light_gray")) {
    return LGRA;
  }
  if (!strcmp(color, "dark_gray")) {
    return DGRA;
  }
}
