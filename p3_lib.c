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

int is_blk_dev(char *path, struct stat *sb) {
  if (file_exists(path)) {
    stat(path, sb);
    return S_ISBLK(sb->st_mode);
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

void copy_file(char *src, char *dst) {
  char buf[BUFSIZ];
  // size_t is used to express the size of an object in bytes. Using ssize_t
  // here to allow for -1 for errors
  ssize_t bytes_read, bytes_written;

  // Check if destination exists. If so, prompt user for overwrite confirmation

  if (file_exists(dst)) {
    char yn[256];
    printf("Destination %s exists. Overwrite? (y/n): ", dst);
    fflush(stdout);
    while (yn[0] != 'y') {
      // Use fgets to avoid leftover newlines in the input buffer
      fgets(yn, 256, stdin);
      if (yn[0] == 'n') {
        fprintf(stderr, "Exiting without copying or moving.\n");
        exit(EXIT_FAILURE);
      } else if (yn[0] != 'y') {
        printf("Please enter 'y' or 'n': ");
        fflush(stdout);
      }
    }
  }

  // Open the files and check for errors

  int src_fd = open(src, O_RDONLY);
  int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (src_fd < 0 || dst_fd < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  // Copy the source to the destination BUFSIZ bytes at a time

  while ((bytes_read = read(src_fd, buf, BUFSIZ)) > 0)
    bytes_written = write(dst_fd, buf, bytes_read);

  // Check for errors and close the files

  if (bytes_read == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  if (bytes_written == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }
  close(src_fd);
  close(dst_fd);
  printf("Successfully copied %s to %s\n", src, dst);
}

void move_file(char *src, char *dst) {
  if (link(src, dst) < 0) {
    perror("link");
    fprintf(stderr, "Can't link to destination %s. Attempting copy...\n", dst);
    copy_file(src, dst);
  }
  if (unlink(src) < 0) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
  printf("Successfully moved %s to %s\n", src, dst);
}
