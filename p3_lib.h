#include <sys/stat.h>
#define HEADER_TEST_VAL 22
void lib_test();
int called_as_copy(char prog_name[]);
int is_reg_file(char *path, struct stat *sb);
int is_dir(char *path, struct stat *sb);
int get_inode_num(char *path, struct stat *sb);
