#include <stdio.h>
#include <string.h>
void lib_test() { printf("hello from lib\n"); }

int called_as_copy(char prog_name[]) {
  return (strcmp(prog_name, "./copy") == 0);
}
