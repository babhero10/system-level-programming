#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"

char *my_export(char *arg) {
  char *equal_sign = strchr(arg, '=');
  if (!equal_sign) return NULL;

  *equal_sign = '\0';
  char *key = arg;
  char *value = equal_sign + 1;

  if (setenv(key, value, 1) == 0) {
    return value;
  } else {
    return NULL;
  }
}
