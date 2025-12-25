#include "file_handler.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int check_file_status(struct file_handle *fh) {
  if (ferror(fh->file) > 0) { return FH_ERR; }

  if (feof(fh->file) > 0) return FH_EOF;

  return FH_OK;
}

// function safely closes struct file_handle
int fh_close(struct file_handle **fh) {
  if (fh == NULL || *fh == NULL) return FH_ERR;

  fclose((*fh)->file);
  free(*fh);
  *fh = NULL;

  return FH_OK;
}
// Function safely opens file_handle
// takes arguments path, mode, and pointer
// status must be a valid pointer.
// On success: *status = FH_OK
// On failure: returns NULL and sets *status accordingly.

struct file_handle *fh_open(const char *path, const char *mode, int *status) {

  FILE *file = fopen(path, mode);
  if (file == NULL) {
    *status = FH_FILE_OPEN_ERR;
    return NULL;
  }

  struct file_handle *handler = malloc(sizeof *handler);
  if (!handler) {
    *status = FH_MALLOC_FAILED;
    return NULL;
  }
  *handler = (struct file_handle){file, false};
  *status = FH_OK;
  return handler;
}

// Function reads char from file_handler and returns it to out.
int fh_read_char(struct file_handle *fh, int *out) {
  if (fh->file == NULL) return FH_CLOSED;
  int c = fgetc(fh->file);
  int status = check_file_status(fh);
  if (status > 0) { return status; }
  if (c == EOF) { return FH_EOF; }
  *out = c;

  return FH_OK;
}

int main() {
  char *path = "./file";
  char *mode = "r";
  int status;

  struct file_handle *fh = fh_open(path, mode, &status);
  int c = 0;
  int s = 0;

  s = fh_read_char(fh, &c);
  while (s == FH_OK) {

    printf("%c", c);
    s = fh_read_char(fh, &c);
  }

  fh_close(&fh);

  return 0;
}
