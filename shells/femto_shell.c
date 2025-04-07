#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../helper/include/helper.h"
#include "../unix_utilities/include/utils.h"

int main(void) {

  int my_argc;
  char **my_argv = NULL;
  char *buffer = NULL;
  size_t len;
  ssize_t ret;

  while (1) {
    printf("Femto shell prompt > ");
    fflush(stdout);

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
