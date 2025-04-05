#include "../include/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void get_args(int *argc, char ***argv, char *text) {
  *argc = 0;
  *argv =
      (char **)malloc((strlen(text) / 2 + 2) * sizeof(char *));
  if (argv == NULL) {
    fprintf(stderr, "Can't allocat more memory\n");
    exit(-1);
  }
  int start_index = 0;
  int end_index = 0;
  unsigned char quote = 0;

  while (1) {

    // Find next token.
    while (' ' == text[start_index]) {
      start_index++;
    }

    // Done
    if (text[start_index] == '\0') {
      (*argv)[(*argc)] = NULL;
      return;
    }

    // Find the end of the token.
    end_index = start_index;
    while (quote == 1 || ' ' != text[end_index]) {
      if (text[end_index] == '\0') {
        break;
      }

      if (text[end_index] == '"') {
        quote ^= 1;
      }
      end_index++;
    }

    int token_len = end_index - start_index;
    (*argv)[(*argc)] = create_string(token_len + 1);
    strncpy((*argv)[(*argc)], &text[start_index], token_len);
    (*argv)[(*argc)][token_len] = '\0';
    clean_string((*argv)[(*argc)]);
    (*argc)++;

    start_index = end_index + 1;
  }
}

void clean_string(char *text) {
  int i_original = 0;
  int i_new = 0;

  for (i_original = 0; text[i_original] != '\0'; i_original++) {
    if (text[i_original] == '"') {
      continue;
    }

    text[i_new++] = text[i_original];
  }

  text[i_new] = '\0';
}

int count_chars_in_string(char *text, char find_chr) {
  if (text == NULL) {
    return -1;
  }

  unsigned char continued = 0;
  int count = 0;
  size_t i;
  for (i = 0; i < strlen(text); i++) {
    if (find_chr == text[i]) {
      if (continued == 0) {
        count++;
      }
      continued = 1;
    } else {
      continued = 0;
    }
  }

  return count;
}

char *create_string(int len) {
  char *new = (char *)malloc(len * sizeof(char));
  if (new == NULL) {
    fprintf(stderr, "Can't allocat more memory\n");
    exit(-1);
  }
  return new;
}

void free_string(char *str) {
  if (str != NULL) {
    free(str);
  }
}

void get_key_value(char *kv_text, char **key, char **value) {
  char *kv_dup = strdup(kv_text);
  *value = strchr(kv_dup, '=');
  if (*value == NULL) {
    free(kv_dup);
    *key = NULL;
    *value = NULL;
    return;
  }

  *(*value) = '\0';
  *key = kv_dup;
  *value = *value + 1;
}


char *lookup_var(char *var, Dictionary *local_var) {
  char *ret = NULL;
  ret = dict_get(local_var, var);

  if (ret != NULL) {
    return ret;
  } 

  ret = getenv(var);

  return ret;
}

char *replace_string(char *src, char *sub_text, int replace_from, int replace_to) {
    int src_len = strlen(src);
    int sub_len = strlen(sub_text);

    // Calculate length of new string
    int new_len = replace_from + sub_len + (src_len - replace_to);
    char *result = malloc(new_len + 1); // +1 for null terminator

    if (!result) return NULL;

    // Copy part before replacement
    strncpy(result, src, replace_from);
    result[replace_from] = '\0';

    // Append the sub_text
    strcat(result, sub_text);

    // Append the rest of the original string after replace_to
    strcat(result, src + replace_to);

    return result;
}

void parse_args(int argc, char *argv[], Dictionary *local_vars) {
  for (int i = 0; i < argc; i++) {
    char *arg = argv[i];
    char *pos = strchr(arg, '$');

    while (pos != NULL) {
      // Move past the $
      char *start = pos + 1;
      if (!isalnum(*start) && *start != '_') {
        // Not a valid var name, skip this $
        pos = strchr(pos + 1, '$');
        continue;
      }

      // Find end of the variable name
      char *end = start;
      while (isalnum(*end) || *end == '_') end++;

      int var_name_len = end - start;
      char var_name[var_name_len + 1];
      strncpy(var_name, start, var_name_len);
      var_name[var_name_len] = '\0';

      char *value = lookup_var(var_name, local_vars);
      if (!value) value = "";

      int from = pos - arg;
      int to = end - arg;

      arg = replace_string(arg, value, from, to);
      pos = strchr(arg + from + strlen(value), '$');
    }

    argv[i] = arg;
  }
}
