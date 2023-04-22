#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void lib_test() { printf("hello from lib\n"); }

int called_as_copy(char prog_name[]) {
  return (strcmp(prog_name, "./copy") == 0);
}

int file_exists(char *path) { return access(path, F_OK) == 0; }

int is_reg_file(char *path, struct stat *sb) {
  if (file_exists(path)) {
    stat(path, sb);
    return S_ISREG(sb->st_mode);
  }
  return 0;
}

int is_dir(char *path, struct stat *sb) {
  if (file_exists(path)) {
    stat(path, sb);
    return S_ISDIR(sb->st_mode);
  }
  return 0;
}

int get_inode_num(char *path, struct stat *sb) {
  if (file_exists(path)) {
    stat(path, sb);
    return sb->st_ino;
  }
  return 0;
}

void move_file(char *src, char *dst) {
  if (link(src, dst) < 0) {
    fprintf(stdout, "Can't link to destination %s\n", dst);
    perror("link");
  }
}

void copy_file(char *src, char *dst) {
  int src_fd = open(src, O_RDONLY);
  int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (src_fd < 0 || dst_fd < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  char buf[BUFSIZ];
  if (read(src_fd, buf, BUFSIZ) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  char c;
}
