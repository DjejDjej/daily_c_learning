#include "file_handler.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Takes *path, *mode, and pointer to status and returns struct file_handle allocated on heap
// on error returns NULL
// on success caller is owner of the file_handle struct
struct file_handle *open_file(char *path, char *mode, int *status) {
  FILE *f = fopen(path, mode);

  if (f == NULL) {
    *status = FH_FILE_OPEN_ERROR;
    return NULL;
  }
  struct file_handle *fh = malloc(sizeof(*fh));

  if (fh == NULL) {
    *status = FH_MALLOC_FAILED;
    return NULL;
  }

  *fh = (struct file_handle){f};

  *status = FH_OK;
  return fh;
}

// takes struct fh, and pointer to status
// on error returns NULL
// on success caller is owner of the sentences struct
struct sentences *get_sentences_from_fh(struct file_handle *fh, int *status) {
  if (fh == NULL) {
    //
    return NULL;
  }
  size_t sent_cap = SEN_SENT_DEF_CAP;
  size_t row_count = 0;
  size_t sent_count = 0;
  size_t row_cap = SEN_ROW_DEF_CAP;
  int c = 0;
  char **sent = malloc(sent_cap * sizeof(char *));
  char *row = malloc(row_cap);
  if (sent == NULL || row == NULL) {
    //
    return NULL;
  }

  while (1) {
    c = fgetc(fh->file);

    if (c == '\n') {
      row[row_count++] = '\0';

      if (sent_cap <= sent_count) {
        sent_cap *= 2;
        if (safe_realloc((void **)&sent, sent_cap, sizeof(sent)) == false) {
          //
          return NULL;
        }
      }
      if (row_cap <= row_count) {
        row_cap *= 2;
        if (safe_realloc((void **)&row, row_cap, sizeof(row)) == false) {
          //
          return NULL;
        }
      }

      sent[sent_count] = malloc(row_count);
      if (sent[sent_count] == NULL) {

        //
        return NULL;
      }

      row_count = 0;
      strcpy(sent[sent_count++], row);

    } else {
      row[row_count++] = c;
    }

    if (c == EOF) { break; }
  }

  free(row);
  return NULL;
}
