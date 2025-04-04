#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/utils.h"


// int main(int argc, char *argv[]) {
//   cp(argc, argv);
//   return 0; 
// }

void cp(int argc, char *argv[]) {
  int ret;
  int source_file_discriptor, dist_file_discriptor;
  char buffer[BUFFER_SIZE];

  if (argc != 3) {
    fprintf(stderr, "Usage: cp source_file dist_file\n");
    exit(-1);
  }

  source_file_discriptor = open(argv[1], O_RDONLY);

  if (source_file_discriptor < 0) {
    fprintf(stderr, "Can't read the following file: %s\n", argv[1]);
    exit(-2);
  }

  dist_file_discriptor = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

  if (dist_file_discriptor < 0) {
    fprintf(stderr, "Can't create the following file: %s\n", argv[2]);
    close(source_file_discriptor);
    exit(-3);
  }

  while (1) {
    ret = read(source_file_discriptor, buffer, 512);

    if (ret == 0) {
      break;
    }

    if (ret < 0) {
      fprintf(stderr, "Couldn't read from the file\n");
      close(source_file_discriptor);
      close(dist_file_discriptor);
      exit(-4);
    }

    if (write(dist_file_discriptor, buffer, ret) < 0) {
      fprintf(stderr, "Couldn't write into the file\n");
      close(source_file_discriptor);
      close(dist_file_discriptor);
      exit(-5);
    }
  }

  close(source_file_discriptor);
  close(dist_file_discriptor);
}
