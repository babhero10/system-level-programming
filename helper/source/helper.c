#include "../include/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
