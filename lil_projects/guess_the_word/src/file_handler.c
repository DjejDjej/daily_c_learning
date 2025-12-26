#include "file_handler.h"
#include "input_handler.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Internal function to free all char **sent.

int fh_free_on_fail(char **sent, size_t count, char *row) {
  for (size_t i = 0; i < count; i++) { free(sent[i]); }
  free(sent);
  free(row);
  return FH_OK;
}
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
struct sentences *get_sentences_from_fh(const struct file_handle *fh, int *status) {
  if (fh == NULL) {
    *status = FH_FILE_OPEN_ERROR;
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
    fh_free_on_fail(sent, sent_count, row);
    *status = FH_MALLOC_FAILED;
    return NULL;
  }

  while (1) {
    c = fgetc(fh->file);
    if (sent_count + 1 >= sent_cap) {
      sent_cap *= 2;
      if (safe_realloc((void **)&sent, sent_cap, sizeof(sent)) == false) {
        *status = FH_REALLOC_FAILED;
        fh_free_on_fail(sent, sent_count, row);
        return NULL;
      }
    }
    if (row_count + 1 >= row_cap) {
      row_cap *= 2;
      if (safe_realloc((void **)&row, row_cap, sizeof(row)) == false) {
        fh_free_on_fail(sent, sent_count, row);
        *status = FH_REALLOC_FAILED;
        return NULL;
      }
    }

    if (c == '\n') {
      row[row_count++] = '\0';
      sent[sent_count] = malloc(row_count);
      if (sent[sent_count] == NULL) {

        *status = FH_MALLOC_FAILED;
        fh_free_on_fail(sent, sent_count, row);
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

  struct sentences *s = malloc(sizeof *s);
    if (s == NULL) {
    *status = MALLOC_FAILED;
    return NULL;
  }

s->data = sent;
  s->count = sent_count;

  *status = FH_OK;
  return s;

}

// function safely closes struct file_handle
int fh_close(struct file_handle **fh) {
  if (fh == NULL || *fh == NULL) return FH_ERR;
  fclose((*fh)->file);
  free(*fh);
  *fh = NULL;

  return FH_OK;
}
// Free all the sentence data
int sentence_free(struct sentences *sent) {
  for (size_t i = 0; i < sent->count; i++) { free(sent->data[i]); }
  free(sent->data);
  free(sent);
  return FH_OK;
}
