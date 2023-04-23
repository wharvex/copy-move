#include "p3_lib.h"
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    if (called_as_copy(argv[0])) {
      fprintf(stderr, "Usage: copy source1 [source2 ...] destination\n");
      return EXIT_FAILURE;
    } else {
      fprintf(stderr, "Usage: move source1 [source2 ...] destination\n");
      return EXIT_FAILURE;
    }
  }
  struct stat sb;
  // When calling as copy or move with more than one source, the destination
  // must be a directory
  if (argc > 3 && !is_dir(argv[argc - 1], &sb)) {
    fprintf(stderr, "ERROR: Destination must be a directory if >1 "
                    "source.\nExiting without copying or moving.\n");
    return EXIT_FAILURE;
  }
  // When calling as copy or move with any number of sources, the destination
  // must be a file or directory or not exist
  if (!is_dir(argv[argc - 1], &sb) && !is_reg_file(argv[argc - 1], &sb) &&
      file_exists(argv[argc - 1])) {
    fprintf(
        stderr,
        "ERROR: Destination must be a file/directory or not exist.\nExiting "
        "without copying or moving.\n");
    return EXIT_FAILURE;
  }
  // Make arg_types string array to identify source(s) and destination
  char arg_types[argc][256];
  strcpy(arg_types[argc - 1], "destination");
  for (int i = 1; i < argc - 1; i++) {
    sprintf(arg_types[i], "source%d", i);
  }
  // Make valid_sources int array to remember which sources exist and are
  // files/directories
  int valid_sources[argc];
  valid_sources[argc - 1] = 0;
  for (int i = 1; i < argc - 1; i++) {
    valid_sources[i] = 1;
  }
  // Print whether called as copy or move
  printf("%s\n", called_as_copy(argv[0]) ? "copy" : "move");
  fflush(stdout);

  // Begin error-checking/details-printing loop

  for (int i = 1; i < argc; i++) {
    // If we're on a source argv and it doesn't exist, print error message and
    // move to the next argv
    if (i < argc - 1 && !file_exists(argv[i])) {
      valid_sources[i] = 0;
      fprintf(stderr, "ERROR: %s (%s) does not exist; skipping...\n\n", argv[i],
              arg_types[i]);
      continue;
    }
    // If we're on a source argv and it isn't a file or directory, print error
    // message and move to the next argv
    if (i < argc - 1 && !is_dir(argv[i], &sb) && !is_reg_file(argv[i], &sb)) {
      valid_sources[i] = 0;
      fprintf(stderr,
              "ERROR: %s (%s) is not a file or directory; skipping...\n\n",
              argv[i], arg_types[i]);
      continue;
    }
    // If there are any duplicate INode numbers in the argv's, print error
    // message and exit
    for (int j = i - 1; j > 0; j--) {
      if (get_inode_num(argv[i], &sb) &&
          get_inode_num(argv[i], &sb) == get_inode_num(argv[j], &sb)) {
        fprintf(stderr,
                "ERROR: %s (%s) is the same as %s (%s)\nExiting without "
                "copying or moving.\n",
                argv[i], arg_types[i], argv[j], arg_types[j]);
        return EXIT_FAILURE;
      }
    }
    // Print file/directory details
    printf("%s%s (%s)\n  Reg file: %s\n  Directory: %s\n  INode num: %d\n\n",
           i == argc - 1 ? "to " : "", argv[i], arg_types[i],
           is_reg_file(argv[i], &sb) ? "yes" : "no",
           is_dir(argv[i], &sb) ? "yes" : "no", get_inode_num(argv[i], &sb));
    fflush(stdout);
  }

  // End error-checking/details-printing loop

  // Count the valid sources. If there is not at least one, error and exit.
  int valid_sources_qty = 0;
  for (int i = 1; i < argc - 1; i++) {
    if (valid_sources[i] == 1) {
      valid_sources_qty++;
    }
  }
  if (!valid_sources_qty) {
    fprintf(stderr, "ERROR: Program needs at least one valid source to run. "
                    "Exiting without copying or moving.\n");
    return EXIT_FAILURE;
  }
  if (called_as_copy(argv[0])) {
    // If argc < 4, then after the error-checks above, the destination will
    // either be a file or not exist
    if (argc < 4) {
      copy_file(argv[1], argv[2]);
    } else {
      char *file_name;
      char path_name[256];
      for (int i = 1; i < argc; i++) {
        if (valid_sources[i]) {
          file_name = basename(argv[i]);
          sprintf(path_name, "%s/%s", argv[argc - 1], file_name);
          copy_file(argv[i], path_name);
        }
      }
    }
  } else {
    if (argc < 4) {
      move_file(argv[1], argv[2]);
    } else {
      char *file_name;
      char path_name[256];
      for (int i = 1; i < argc; i++) {
        if (valid_sources[i]) {
          file_name = basename(argv[i]);
          sprintf(path_name, "%s/%s", argv[argc - 1], file_name);
          move_file(argv[i], path_name);
        }
      }
    }
  }
  return EXIT_SUCCESS;
}
