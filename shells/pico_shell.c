#define _GNU_SOURCE
#include "../helper/include/helper.h"
#include "../unix_utilities/include/utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

  int my_argc;
  char **my_argv = NULL;
  char *buffer = NULL;
  size_t len;
  ssize_t ret;
  int i = 0;

  while (1) {
    printf("Pico shell prompt > ");

    ret = getline(&buffer, &len, stdin);
    if (ret == -1) {
      if (errno != 0) {
        fprintf(stderr, "Error occurred while reading input: %s.\n",
                strerror(errno));
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
      if (strcmp(my_argv[0], "pwd") == 0) {
        pwd();
      } else if (strcmp(my_argv[0], "echo") == 0) {
        echo(my_argc, my_argv);
      } else if (strcmp(my_argv[0], "mv") == 0) {
        mv(my_argc, my_argv);
      } else if (strcmp(my_argv[0], "cp") == 0) {
        cp(my_argc, my_argv);
      } else if (strcmp(my_argv[0], "exit") == 0) {
        printf("Good Bye!\n");
        break;
      } else if (strcmp(my_argv[0], "clear") == 0) {
        printf("\033[H\033[J");
      } else {
        pid_t pid = fork();
        if (pid < 0) {
          fprintf(stderr, "Can't create the process\n");
        } else if (pid > 0) {
          int status;
          wait(&status);
          WEXITSTATUS(status);
        } else {
          char *newenvp[] = {NULL};
          execvpe(my_argv[0], my_argv, newenvp);
          exit(-1);
        }
      }
    }

    for (i = 0; i < my_argc; i++) {
      free(my_argv[i]);
    }

    free(my_argv);
    my_argv = NULL;
    free(buffer);
    buffer = NULL;
  }

  return 0;
}
