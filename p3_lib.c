#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
void lib_test() { printf("hello from lib\n"); }

int called_as_copy(char prog_name[]) {
  return (strcmp(prog_name, "./copy") == 0);
}

int is_reg_file(char *path, struct stat *sb) {
  if (access(path, F_OK) == 0) {
    stat(path, sb);
    return S_ISREG(sb->st_mode);
  }
  return 0;
}

int is_dir(char *path, struct stat *sb) {
  if (access(path, F_OK) == 0) {
    stat(path, sb);
    return S_ISDIR(sb->st_mode);
  }
  return 0;
}

int get_inode_num(char *path, struct stat *sb) {
  if (access(path, F_OK) == 0) {
    stat(path, sb);
    return sb->st_ino;
  }
  return 0;
}
