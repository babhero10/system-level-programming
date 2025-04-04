#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/utils.h"

void parse_string(char text[]);
void parse_special_char(char special_char);

// int main(int argc, char *argv[]) {
//   echo(argc, argv);
//   return 0;
// }

void echo(int argc, char *argv[]) {
  int i;

  for (i = 1; i < argc; i++) {
    parse_string(argv[i]);
    if (i < argc - 1){
        printf(" ");
    }
  }

  printf("\n");
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

void parse_special_char(char special_char) {
    switch (special_char) {
        case 'n': printf("\n"); break;
        case 't': printf("\t"); break;
        case 'r': printf("\r"); break;
        case 'b': printf("\b"); break;
        case 'f': printf("\f"); break;
        case 'v': printf("\v"); break;
        default: printf("%c", special_char); break;
    }
}
