#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../include/utils.h"

// int main(int argc, char *argv[]) {
//   mv(argc, argv);
//   return 0;
// }

void mv(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: mv source_file dist_file\n");
    exit(-1);
  }

  if (rename(argv[1], argv[2]) < 0) {
    if (errno == ENOENT) {
      fprintf(stderr,
              "Can't find following file: %s\n", argv[1]);
      exit(-1);
    }
  }
}
