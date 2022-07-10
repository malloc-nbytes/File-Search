#include "hash_table.h"
#include "stack.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void panic(char *err_msg) {
  fprintf(stderr, "ERROR: %s\n", err_msg);
  exit(EXIT_FAILURE);
}

void validate_string(char *data) {
  if (!data) {
    panic("string failed to malloc.");
  }
}

void validate_directory(DIR *dir) {
  if (!dir) {
    panic("dir validation failed.");
  }
}

char *concat_cwd(char *cwd, char *file_name) {
  int cwd_sz       = strlen(cwd);
  int file_name_sz = strlen(file_name);
  char *res;

  // +1 for '/' and +1 for NULL.
  if (!(res = (char *)malloc(cwd_sz + file_name_sz + 1 + 1))) {
    return NULL;
  }

  // Concat "/filename".
  strcpy(res, cwd);
  res[cwd_sz] = '/';
  strcat(res, file_name);
  return res;
}

void free_mem(Stack *stack, HashTable *hash_table, DIR *dir) {
  stack_free(stack);
  hashtable_free(hash_table);
  closedir(dir);
}

#define HIDDEN(x) (x[0] == '.')
#define VALID_DIR(x) (strcmp(x, "..") != 0 && strcmp(x, "."))
#define IS_DIR(x) (x & DT_DIR)
#define CWD_BUF_CAP 2052

Stack *file_search(char *file_name) {

  Stack *stack          = stack_alloc();
  Stack *results        = stack_alloc();
  HashTable *hash_table = hashtable_alloc();

  char cwd_buf[CWD_BUF_CAP];
  struct dirent *dirent;
  DIR *dir;

  memset(cwd_buf, '\0', CWD_BUF_CAP * sizeof(cwd_buf[0]));

  while (strcmp(getcwd(cwd_buf, CWD_BUF_CAP), "/") != 0) {
    dir = opendir(".");
    validate_directory(dir);

    // Search the current directory.
    while ((dirent = readdir(dir))) {

      // The current item is a valid, non-hidden directory.
      if (IS_DIR(dirent->d_type) && VALID_DIR(dirent->d_name) &&
          !HIDDEN(dirent->d_name)) {

        // Add '/filename' to the directory.
        char *cwd_with_filename = concat_cwd(cwd_buf, dirent->d_name);
        validate_string(cwd_with_filename);

        // Put it into the hashtable. If it is already there, skip. Otherwise,
        // put it into the stack.
        hashtable_put(hash_table, cwd_with_filename)
            ? stack_push(stack, cwd_with_filename)
            : free(cwd_with_filename);
      }

      // Found the file.
      if (strcmp(file_name, dirent->d_name) == 0 && !IS_DIR(dirent->d_type)) {
        // Push the directory into the results stack.
        char *tmp = (char *)malloc(sizeof(cwd_buf));
        validate_string(tmp);
        strcpy(tmp, cwd_buf);
        stack_push(results, tmp);
      }
    }

    // If there is something in the stack, switch to that directory.
    if (!stack_empty(stack)) {
      chdir(stack_pop(stack));
    }

    // Otherwise, navigate back a directory.
    else {
      chdir("..");
    }
  }

  // Cleanup.
  free_mem(stack, hash_table, dir);
  return results;
}

void check_args(int argc) {
  if (argc < 2 || argc > 2) {
    panic("usage: ./file_search <filename>");
  }
}

int main(int argc, char **argv) {

  check_args(argc);
  argv++;
  char *file_name = *argv++;

  chdir("/home/");

  Stack *results = file_search(file_name);

  if (stack_empty(results)) {
    printf("File: [%s] not found.\n", file_name);
  } else {
    printf("File: [%s] was found in the following directories:\n", file_name);
    stack_dump(results);
  }

  stack_free(results);

  return 0;
}
