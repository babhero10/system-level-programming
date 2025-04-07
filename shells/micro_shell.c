#define _GNU_SOURCE
#include "../helper/include/helper.h"
#include "../unix_utilities/include/utils.h"
#include <errno.h>
#include <fcntl.h>
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
  Dictionary *local_vars = dict_create(16);
  char *key = NULL, *value = NULL;
  int first_redirect = -1;
  int saved_stdin = dup(STDIN_FILENO);
  int saved_stdout = dup(STDOUT_FILENO);
  int saved_stderr = dup(STDERR_FILENO);

  int in_fd, out_fd;

  while (1) {
    first_redirect = -1;
    close(in_fd);
    close(out_fd);
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);

    printf("Micro shell prompt > ");
    fflush(stdout);

    ret = getline(&buffer, &len, stdin);
    if (ret < 0) {
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
    parse_args(my_argc, my_argv, local_vars);
    if (my_argc > 0) {
      for (i = 1; i < my_argc; i++) {
        if (strcmp(my_argv[i], "<") == 0) {
          if (first_redirect == -1) {
            first_redirect = i;
          }
          if (i + 1 < my_argc) {
            in_fd = open(my_argv[i + 1], O_RDONLY);
            if (in_fd < 0) {
              perror("Failed to open input file");
              first_redirect = -2;
              break;
            }
            if (dup2(in_fd, 0) < 0) {
              perror("dup2 for input failed");
              close(in_fd);
              first_redirect = -2;
              break;
            }
          } else {
            first_redirect = -2;
            break;
          }
        } else if (strcmp(my_argv[i], ">") == 0) {
          if (first_redirect == -1) {
            first_redirect = i;
          }
          if (i + 1 < my_argc) {
            out_fd = open(my_argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) {
              perror("Failed to open/create output file");
              first_redirect = -2;
              break;
            }
            if (dup2(out_fd, 1) < 0) {
              perror("dup2 for input failed");
              close(out_fd);
              first_redirect = -2;
              break;
            }
          } else {
            first_redirect = -2;
            break;
          }
        } else if (strcmp(my_argv[i], "2>") == 0) {
          if (first_redirect == -1) {
            first_redirect = i;
          }
          if (i + 1 < my_argc) {
            int out_fd =
                open(my_argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) {
              perror("Failed to open/create output file");
              first_redirect = -2;
              break;
            }
            if (dup2(out_fd, 2) < 0) {
              perror("dup2 for input failed");
              close(out_fd);
              first_redirect = -2;
              break;
            }
          } else {
            first_redirect = -2;
            break;
          }
        }
      }

      if (first_redirect != -1) {
        my_argc = first_redirect;
        my_argv[first_redirect] = NULL;
      }
      if (first_redirect != -2) {

        if (strchr(my_argv[0], '=') != NULL && my_argc < 2) {
          get_key_value(my_argv[0], &key, &value);
          dict_put(local_vars, key, value);
        } else if (strcmp(my_argv[0], "pwd") == 0) {
          pwd();
        } else if (strcmp(my_argv[0], "echo") == 0) {
          echo(my_argc, my_argv);
        } else if (strcmp(my_argv[0], "mv") == 0) {
          mv(my_argc, my_argv);
        } else if (strcmp(my_argv[0], "cp") == 0) {
          cp(my_argc, my_argv);
        } else if (strcmp(my_argv[0], "export") == 0) {
          char *tmp = my_export(my_argv[1]);
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
            if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
              fprintf(stderr, "invalid command: %s\n", my_argv[0]);
            }
          } else {
            execvp(my_argv[0], my_argv);
            exit(127);
          }
        }
      }
    }

    if (key != NULL) {
      free(key);
      key = NULL;
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
