#include "p3_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    if (called_as_copy(argv[0]))
      printf("Usage: copy source1 [source2 ...] destination\n");
    else
      printf("Usage: move source1 [source2 ...] destination\n");
  }

  return EXIT_SUCCESS;
}
