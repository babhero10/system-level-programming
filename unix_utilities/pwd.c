#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(void)
{
  char buf[BUFFER_SIZE];
  char *res;

  res = getcwd(buf, BUFFER_SIZE) ;

  if (res == NULL)
  {
    if (errno == EACCES)
    {
      fprintf(stderr, "You don't have the premssion to access the directory!\n");
      exit(-1);
    }
    else if (errno == ERANGE)
    {
      fprintf(stderr, "Ops!, Buffer reach it limit!\nThis problem can be solved later by reallocate the buffer with bigger size!\n"
              "get_current_dir_name() can be used also but will require additional free!\n");
      exit(-2);
    }
    else
    {
      fprintf(stderr, "Something went wrong!\n");
      exit(-3);
    }
  }

  printf("%s\n", res);

  return 0;
}
