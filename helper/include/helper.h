#ifndef HELPHER_H
#define HELPHER_H

void get_args(int *argc, char ***argv, char *text);
int count_chars_in_string(char *text, char find_chr);
char *create_string(int len);
void free_string(char *str);
char *parse_string(char text[]);
void clean_string(char *text);

#endif /* HELPHER_H */
