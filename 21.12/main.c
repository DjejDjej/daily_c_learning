#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_DEF_SIZE 10
#define ALL_CHARS_COUNT_DEF 100

//TODO FIX MALLOC OF **result 
//TOOD PRIDAT ROLLBACK

enum status_codes {
  IN_PROGRESS = -1,
  OK = 0,
  REALLOC_FAILED = 1,
  FILE_NULL = 2,
  MALLOC_FAILED = 3,

};

struct result_lines {
  int status_code;
  char **lines;
  size_t count;
};

int freeAll(struct result_lines l) {

  for (size_t i = 0; i < l.count; i++) {

    printf("%s\n", l.lines[i]);
    free(l.lines[i]);
  }
  free(l.lines);
  return 0;
}

int safe_realloc(char **buff, size_t size) {

  void *tmp = realloc(*buff, size);
  if (tmp == NULL) {
    return REALLOC_FAILED;
  } else {
    *buff = tmp;
    return 0;
  }
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
  size_t buff_size = LINE_DEF_SIZE;
  size_t all_chars_alloc = ALL_CHARS_COUNT_DEF;
  size_t all_chars_count = 0;

  char *buff = malloc(buff_size);
  char **result = malloc(all_chars_alloc * sizeof(char * ));

  if (buff == NULL || result == NULL) {
    r.status_code = MALLOC_FAILED;
    return r;
  }

  int ch;
  int i = 0;
  int count = 0;
  size_t psize = sizeof(char * );
  while ((ch = fgetc(f)) != EOF) {
    if (buff_size + 1 <= i) {
      buff_size *= 2;
      if (safe_realloc(&buff, buff_size) == REALLOC_FAILED) {
        r.status_code = REALLOC_FAILED;
        return r;
      }
    }

    if (all_chars_alloc <= all_chars_count) {
      all_chars_alloc *= 2;
      if (safe_realloc_Ar(&result, all_chars_alloc) == REALLOC_FAILED) {
        r.status_code = REALLOC_FAILED;
        return r;
      }
    }
    if (ch == '\n' || ch == '\0') {
      buff[i] = '\0';
      result[count] = malloc(buff_size);
      if(result[count] == NULL) {
       r.status_code = MALLOC_FAILED;
	return r;
      }
      strncpy(result[count++], buff, buff_size);
      i = 0;
      buff_size = LINE_DEF_SIZE;
      free(buff);
      buff = malloc(buff_size);

    } else {

      buff[i++] = ch;
      all_chars_count+=psize;
    }
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

  printf("%lu\n", l.count);

  freeAll(l);
  return 0;
}
