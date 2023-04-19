#include "p3_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    if (called_as_copy(argv[0]))
      printf("Usage: copy source1 [source2 ...] destination\n");
    else
      printf("Usage: move source1 [source2 ...] destination\n");
  } else {
    struct stat sb;
    for (int i = 1; i < argc; i++) {
      printf("%s\n  Reg file: %s\n  Directory: %s\n  INode num: %d\n\n",
             argv[i], is_reg_file(argv[i], &sb) ? "yes" : "no",
             is_dir(argv[i], &sb) ? "yes" : "no", get_inode_num(argv[i], &sb));
      for (int j = i - 1; j > 0; j--) {
        if (get_inode_num(argv[i], &sb) == get_inode_num(argv[j], &sb)) {
          printf("%s is the same as %s\n", argv[i], argv[j]);
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
