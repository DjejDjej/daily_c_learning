#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_DEF_SIZE 10
#define LINE_COUNT_DEF 10

enum status_codes {
  IN_PROGRESS = -1,
  OK = 0,
  REALLOC_FAILED = 1,
  FILE_NULL = 2,
  MALLOC_FAILED = 3,
  EOF_ON_START = 4,

};

struct result_lines {
  int status_code;
  char **lines;
  size_t count;
};

int freeAll(struct result_lines l) {

  for (size_t i = 0; i < l.count; i++) { free(l.lines[i]); }
  free(l.lines);
  return 0;
}

int cleanUP(char **result, size_t count, char *buff) {

  for (size_t i = 0; i < count; i++) free(result[i]);

  free(result);
  free(buff);
  return 0;
}



int safe_realloc_Ar(char ***buff, size_t size) {

  void *tmp = realloc(*buff, size * sizeof(char *));
  if (tmp == NULL) {
    return REALLOC_FAILED;
  } else {
    *buff = tmp;
    return 0;
  }
}

struct result_lines readlines(FILE *f) {

  struct result_lines r = {-1, NULL, 0};
  if (f == NULL) {
    r.status_code = FILE_NULL;
    return r;
  }

  size_t buff_size = LINE_DEF_SIZE;
  size_t lines_capacity = LINE_COUNT_DEF;

  char *buff = malloc(buff_size);
  char **result = malloc(lines_capacity * sizeof(char *));

  if (buff == NULL || result == NULL) {
    r.status_code = MALLOC_FAILED;
    free(buff);
    free(result);
    return r;
  }

  int ch;
  int i = 0;
  int count = 0;
  ch = fgetc(f);
  if (ch == EOF) {
    r.status_code = EOF_ON_START;
    free(result);
    return r;
  }
  while (1) {

    if (i + 1 >= buff_size) {
      buff_size *= 2;
      if (safe_realloc(&buff, buff_size) == REALLOC_FAILED) {
        r.status_code = REALLOC_FAILED;
        cleanUP(result, count, buff);
        return r;
      }
    }

    if (count >= lines_capacity) {
      lines_capacity *= 2;
      if (safe_realloc_Ar(&result, lines_capacity) == REALLOC_FAILED) {
        r.status_code = REALLOC_FAILED;
        cleanUP(result, count, buff);
        return r;
      }
    }
    if (ch == '\n' || ch == EOF) {
      buff[i] = '\0';
      result[count] = malloc(buff_size);
      if (result[count] == NULL) {
        r.status_code = MALLOC_FAILED;
        cleanUP(result, count, buff);
        return r;
      }
      strcpy(result[count++], buff);
      i = 0;
      buff_size = LINE_DEF_SIZE;

    } else {
      buff[i++] = ch;
    }
    if ((ch = fgetc(f)) == EOF) { break; }
  }

  free(buff);
  r.lines = result;
  r.count = count;
  r.status_code = OK;
  return r;
}

int main() {

  FILE *f = fopen("file", "r");
  struct result_lines l = readlines(f);
  if (l.status_code != OK) { return 1; }

  for (size_t i = 0; i < l.count; i++) { printf("%s", l.lines[i]); }

  freeAll(l);
  return 0;
}
