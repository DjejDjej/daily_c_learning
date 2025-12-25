
#ifndef GUESS_FILE_HANDLER_H
#define GUESS_FILE_HANDLER_H
#include <stdio.h>

#define SEN_ROW_DEF_CAP 10
#define SEN_SENT_DEF_CAP 10

enum fh_status_code {
  FH_OK = 0,
  FH_MALLOC_FAILED = 1,
  FH_FILE_OPEN_ERROR = 2,

};

struct file_handle {
  FILE *file;
};
struct sentences {
  char **sentences;
  size_t count;
};

struct file_handle *open_file(char *path, char *mode, int *status);
struct sentences *get_sentences_from_fh(struct file_handle *fh, int *status);
#endif
