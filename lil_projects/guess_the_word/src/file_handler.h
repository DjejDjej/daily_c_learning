
#ifndef GUESS_FILE_HANDLER_H
#define GUESS_FILE_HANDLER_H
#include <stdio.h>

#define SEN_ROW_DEF_CAP 10
#define SEN_SENT_DEF_CAP 10

enum fh_status_code {
  FH_OK = 0,
  FH_MALLOC_FAILED = 1,
  FH_FILE_OPEN_ERROR = 2,
  FH_REALLOC_FAILED = 3,
  FH_ERR = 4,

};

struct file_handle {
  FILE *file;
};
struct sentences {
  char **data;
  size_t count;
};

int fh_close(struct file_handle **fh);
int sentence_free(struct sentences *sent);
struct file_handle *open_file(char *path, char *mode, int *status);
struct sentences *get_sentences_from_fh(const struct file_handle *fh, int *status);
#endif
