#ifndef HELPHER_H
#define HELPHER_H

#include "dict.h"

void get_args(int *argc, char ***argv, char *text);
int count_chars_in_string(char *text, char find_chr);
char *create_string(int len);
void free_string(char *str);
char *parse_string(char text[]);
void clean_string(char *text);
void get_key_value(char *kv_text, char **key, char **value);
void parse_args(int argc, char *argv[], Dictionary *local_vars);

#endif /* HELPHER_H */
