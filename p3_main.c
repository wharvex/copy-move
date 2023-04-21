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
    char arg_types[argc][256];
    strcpy(arg_types[argc - 1], "destination");
    for (int i = 1; i < argc - 1; i++) {
      sprintf((char *)arg_types[i], "source%d", i);
    }
    printf("%s ", called_as_copy(argv[0]) ? "copy" : "move");
    for (int i = 1; i < argc; i++) {
      printf("%s%s (%s)\n  Reg file: %s\n  Directory: %s\n  INode num: %d\n\n",
             i == argc - 1 ? "to " : "", argv[i], arg_types[i],
             is_reg_file(argv[i], &sb) ? "yes" : "no",
             is_dir(argv[i], &sb) ? "yes" : "no", get_inode_num(argv[i], &sb));
      for (int j = i - 1; j > 0; j--) {
        if (get_inode_num(argv[i], &sb) == get_inode_num(argv[j], &sb)) {
          printf("%s (%s) is the same as %s (%s)\n\n", argv[i], arg_types[i],
                 argv[j], arg_types[j]);
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
