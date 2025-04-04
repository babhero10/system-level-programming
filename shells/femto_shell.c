#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void get_args(int *argc, char **argv[], char *text);
void echo(int argc, char *argv[]);

int main(void) {

  int my_argc;
  char **my_argv = NULL;
  char *buffer = NULL;
  size_t len;
  ssize_t ret;

  while (1) {
    printf("Femto shell prompt > ");

    ret = getline(&buffer, &len, stdin);
    if (ret == -1) {
      if (errno != 0) {
        fprintf(stderr, "Error occurred while reading input: %s.\n", strerror(errno));
        free(buffer);
        exit(-1);
      } else {
        free(buffer);
        return 0;
      }
    } else {
      buffer[strlen(buffer) - 1] = '\0';
    }

    get_args(&my_argc, &my_argv, buffer);

    if (my_argc > 0) {
      if (strcmp(my_argv[0], "echo") == 0) {
        echo(my_argc, my_argv);
      } else if (strcmp(my_argv[0], "exit") == 0) {
        printf("Good Bye! ;)\n");
        break;
      } else if (strcmp(my_argv[0], "clear") == 0) {
        printf("\033[H\033[J");
      } else {
        printf("Invalid command: %s\n", my_argv[0]);
      }
    }

    free(my_argv);
    my_argv = NULL;
    free(buffer);
    buffer = NULL;
  }

  free(my_argv);
  free(buffer);
  return 0;
}

void get_args(int *argc, char ***argv, char *text) {
  *argv = (char **)malloc((strlen(text) / 2) * sizeof(char *));
  *argc = 0;

  char *next_arg = strtok(text, " ");

  while (next_arg != NULL) {
    (*argv)[(*argc)++] = next_arg;
    next_arg = strtok(NULL, " ");
  }
}

void parse_special_char(char special_char) {
  switch (special_char) {
  case 'n':
    printf("\n");
    break;
  case 't':
    printf("\t");
    break;
  case 'r':
    printf("\r");
    break;
  case 'b':
    printf("\b");
    break;
  case 'f':
    printf("\f");
    break;
  case 'v':
    printf("\v");
    break;
  default:
    printf("%c", special_char);
    break;
  }
}

void parse_string(char text[]) {
  const int len = strlen(text);
  int i;

  for (i = 0; i < len; i++) {
    if (text[i] != '\\') {
      fputc(text[i], stdout);
    } else if (i + 1 < len) {
      i++;
      parse_special_char(text[i]);
    }
  }
}

void echo(int argc, char *argv[]) {
  int i;

  for (i = 1; i < argc; i++) {
    parse_string(argv[i]);
    if (i < argc - 1) {
      printf(" ");
    }
  }

  printf("\n");
}
